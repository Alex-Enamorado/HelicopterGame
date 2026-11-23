#include "JuegoScene.h"
#include "Helicoptero.h"
#include "Configuracion.h"
#include "Sprites.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QBrush>
#include <QPen>

JuegoScene::JuegoScene(QObject *padre)
    : QGraphicsScene(padre)
{
    // Área delimitada del juego: todo ocurre dentro de este rectángulo.
    setSceneRect(0, 0, Config::ANCHO_ESCENA, Config::ALTO_ESCENA);
    setBackgroundBrush(Qt::black);

    // El orden de creación no importa para el dibujo porque cada item
    // tiene su propio zValue (profundidad).
    crearFondo();
    crearTecho();
    crearSuelo();
    crearHelicoptero();
    crearHUD();
    crearCapaMensaje();

    reiniciar();
}

JuegoScene::~JuegoScene()
{
    // No se hace delete de los items: QGraphicsScene los libera en su
    // propio destructor. Solo se anulan los punteros para dejar claro que
    // ya no deben usarse (evita punteros colgantes si se añadiera código aquí).
    fondo = nullptr;
    techo = nullptr;
    suelo = nullptr;
    helicoptero = nullptr;
    panelHUD = nullptr;
    textoHUD = nullptr;
    capaMensaje = nullptr;
    textoMensaje = nullptr;
}

// ---------------------------------------------------------------------------
// Construcción del escenario
// ---------------------------------------------------------------------------

void JuegoScene::crearFondo()
{
    const QSize tamEscena(Config::ANCHO_ESCENA, Config::ALTO_ESCENA);
    QPixmap lienzo(tamEscena);
    lienzo.fill(QColor(18, 22, 28));

    QPainter p(&lienzo);
    QPixmap cueva(":/img/fondo_cueva.png");
    if (!cueva.isNull()) {
        // Se escala cubriendo toda la escena (puede recortar los bordes)
        // y se centra.
        QPixmap escalada = cueva.scaled(tamEscena, Qt::KeepAspectRatioByExpanding,
                                        Qt::SmoothTransformation);
        const int dx = (escalada.width() - tamEscena.width()) / 2;
        const int dy = (escalada.height() - tamEscena.height()) / 2;
        p.drawPixmap(0, 0, escalada, dx, dy, tamEscena.width(), tamEscena.height());
    } else {
        // Fondo temporal: degradado oscuro tipo cueva
        QLinearGradient degradado(0, 0, 0, tamEscena.height());
        degradado.setColorAt(0.0, QColor(30, 26, 22));
        degradado.setColorAt(0.5, QColor(20, 40, 55));
        degradado.setColorAt(1.0, QColor(30, 26, 22));
        p.fillRect(lienzo.rect(), degradado);
    }
    // Velo oscuro para que el helicóptero resalte sobre el fondo
    p.fillRect(lienzo.rect(), QColor(0, 0, 0, 70));
    p.end();

    fondo = new QGraphicsPixmapItem(lienzo);
    fondo->setZValue(0);
    addItem(fondo);   // A partir de aquí la escena es la dueña
}

QPixmap JuegoScene::construirFranja(const QString &rutaA, const QString &rutaB,
                                    int ancho, int alto,
                                    const QColor &colorRespaldo)
{
    QPixmap franja(ancho, alto);
    franja.fill(Qt::transparent);

    QPixmap seccionA = Sprites::cargar(rutaA);
    QPixmap seccionB = Sprites::cargar(rutaB);

    QPainter p(&franja);
    if (seccionA.isNull() || seccionB.isNull()) {
        // Recursos faltantes: franja sólida temporal
        p.fillRect(franja.rect(), colorRespaldo);
        p.end();
        return franja;
    }

    // Ambas secciones se escalan a la misma altura
    seccionA = seccionA.scaledToHeight(alto, Qt::SmoothTransformation);
    seccionB = seccionB.scaledToHeight(alto, Qt::SmoothTransformation);

    // Se alternan A, B, A, B... hasta cubrir todo el ancho.
    // Se solapan unos píxeles para que no queden huecos entre secciones.
    const int solape = 6;
    int x = 0;
    bool usarA = true;
    while (x < ancho) {
        const QPixmap &actual = usarA ? seccionA : seccionB;
        p.drawPixmap(x, 0, actual);
        x += actual.width() - solape;
        usarA = !usarA;
    }
    p.end();
    return franja;
}

void JuegoScene::crearTecho()
{
    const int alto = static_cast<int>(Config::ALTO_TECHO);
    QPixmap imagen = construirFranja(":/img/techo_1.png", ":/img/techo_2.png",
                                     Config::ANCHO_ESCENA, alto,
                                     QColor(110, 75, 45));
    techo = new QGraphicsPixmapItem(imagen);
    techo->setPos(0, 0);
    // La colisión respeta la forma de las estalactitas
    techo->setShapeMode(QGraphicsPixmapItem::MaskShape);
    techo->setZValue(5);
    addItem(techo);
}

void JuegoScene::crearSuelo()
{
    const int alto = static_cast<int>(Config::ALTO_SUELO);
    QPixmap imagen = construirFranja(":/img/suelo_1.png", ":/img/suelo_2.png",
                                     Config::ANCHO_ESCENA, alto,
                                     QColor(120, 85, 50));
    suelo = new QGraphicsPixmapItem(imagen);
    suelo->setPos(0, Config::ALTO_ESCENA - alto);
    suelo->setShapeMode(QGraphicsPixmapItem::MaskShape);
    suelo->setZValue(5);
    addItem(suelo);
}

void JuegoScene::crearHelicoptero()
{
    helicoptero = new Helicoptero();
    addItem(helicoptero);
}

void JuegoScene::crearHUD()
{
    panelHUD = new QGraphicsRectItem(0, 0, 330, 104);
    panelHUD->setPos(Config::ANCHO_ESCENA - 342, 10);
    panelHUD->setBrush(QColor(15, 20, 12, 190));
    panelHUD->setPen(QPen(QColor(201, 180, 88), 2));
    panelHUD->setZValue(20);
    addItem(panelHUD);

    // El texto es hijo del panel: se mueve con él y el panel lo libera.
    textoHUD = new QGraphicsSimpleTextItem(panelHUD);
    QFont fuente("Consolas", 11, QFont::Bold);
    fuente.setStyleHint(QFont::Monospace);
    textoHUD->setFont(fuente);
    textoHUD->setBrush(QColor(220, 230, 200));
    textoHUD->setPos(12, 8);
}

void JuegoScene::crearCapaMensaje()
{
    capaMensaje = new QGraphicsRectItem(0, 0, Config::ANCHO_ESCENA, Config::ALTO_ESCENA);
    capaMensaje->setPen(Qt::NoPen);
    capaMensaje->setZValue(50);
    addItem(capaMensaje);

    textoMensaje = new QGraphicsTextItem(capaMensaje);
    textoMensaje->setDefaultTextColor(Qt::white);
    textoMensaje->setTextWidth(Config::ANCHO_ESCENA);
}

// ---------------------------------------------------------------------------
// Partida
// ---------------------------------------------------------------------------

void JuegoScene::reiniciar()
{
    tiempoVuelo = 0.0;
    if (helicoptero)
        helicoptero->reiniciar(QPointF(Config::HELI_POS_X_INICIAL,
                                       Config::HELI_POS_Y_INICIAL));
    ocultarMensaje();
    actualizarHUD();
}

void JuegoScene::actualizar(double deltaTime)
{
    if (!helicoptero)
        return;

    tiempoVuelo += deltaTime;
    helicoptero->actualizar(deltaTime);
    actualizarHUD();
}

JuegoScene::Impacto JuegoScene::verificarColisiones() const
{
    if (!helicoptero)
        return Impacto::Ninguno;

    // 1) Colisión precisa por forma (silueta del sprite contra la silueta
    //    del techo / suelo).
    if (techo && helicoptero->collidesWithItem(techo, Qt::IntersectsItemShape))
        return Impacto::Techo;
    if (suelo && helicoptero->collidesWithItem(suelo, Qt::IntersectsItemShape))
        return Impacto::Suelo;

    // 2) Red de seguridad: si por cualquier motivo sale del área de juego,
    //    también cuenta como impacto.
    const QRectF caja = helicoptero->sceneBoundingRect();
    if (caja.top() <= 0.0)
        return Impacto::Techo;
    if (caja.bottom() >= Config::ALTO_ESCENA)
        return Impacto::Suelo;

    return Impacto::Ninguno;
}

void JuegoScene::actualizarHUD()
{
    if (!textoHUD || !helicoptero)
        return;

    // Altura medida desde la superficie del suelo hasta el centro del helicóptero
    const double altura = (Config::ALTO_ESCENA - Config::ALTO_SUELO) - helicoptero->getPosY();

    const QString texto = QString(
        "TIEMPO DE VUELO : %1 s\n"
        "ALTURA          : %2 px\n"
        "VELOCIDAD Y     : %3 px/s\n"
        "ACELERACION     : %4 px/s2\n"
        "MOTOR           : %5")
        .arg(tiempoVuelo, 0, 'f', 1)
        .arg(altura, 0, 'f', 0)
        .arg(helicoptero->getVelocidadY(), 0, 'f', 0)
        .arg(helicoptero->getAceleracion(), 0, 'f', 0)
        .arg(helicoptero->tieneImpulso() ? "IMPULSO" : "LIBRE");

    textoHUD->setText(texto);
}

void JuegoScene::mostrarMensaje(const QString &titulo, const QString &detalle,
                                bool oscurecerFondo)
{
    if (!capaMensaje || !textoMensaje)
        return;

    capaMensaje->setBrush(oscurecerFondo ? QColor(0, 0, 0, 160)
                                         : QColor(0, 0, 0, 0));

    textoMensaje->setHtml(QString(
        "<div align='center' style='font-family:Consolas,monospace;'>"
        "<div style='font-size:40px; font-weight:bold; color:#e8d27a;'>%1</div>"
        "<div style='font-size:18px; color:#f0f0e0;'>%2</div>"
        "</div>").arg(titulo, detalle));

    // Centrado vertical del bloque de texto
    const double alto = textoMensaje->boundingRect().height();
    textoMensaje->setPos(0, (Config::ALTO_ESCENA - alto) / 2.0);

    capaMensaje->setVisible(true);
}

void JuegoScene::ocultarMensaje()
{
    if (capaMensaje)
        capaMensaje->setVisible(false);
}
