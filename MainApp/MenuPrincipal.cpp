#include "MenuPrincipal.h"

#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPushButton>
#include <QRadialGradient>
#include <QVBoxLayout>

MenuPrincipal::MenuPrincipal(QWidget *padre)
    : QWidget(padre),
      imagenFondo(":/img/fondo_montanas.png")
{
    // ----- Decoración: helicóptero sobre el título --------------------------
    QLabel *iconoHeli = new QLabel(this);
    QPixmap heli(":/img/helicoptero.png");
    if (!heli.isNull())
        iconoHeli->setPixmap(heli.scaledToWidth(150, Qt::SmoothTransformation));
    iconoHeli->setAlignment(Qt::AlignCenter);

    // ----- Título -----------------------------------------------------------
    QLabel *titulo = new QLabel("HELICOPTER RESCUE", this);
    QFont fuenteTitulo("Impact", 52);
    fuenteTitulo.setStyleHint(QFont::SansSerif);
    fuenteTitulo.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    titulo->setFont(fuenteTitulo);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("color: #e8d27a;");

    // Sombra del título. setGraphicsEffect transfiere la propiedad al label.
    QGraphicsDropShadowEffect *sombra = new QGraphicsDropShadowEffect();
    sombra->setBlurRadius(18);
    sombra->setOffset(3, 4);
    sombra->setColor(QColor(0, 0, 0, 220));
    titulo->setGraphicsEffect(sombra);

    QLabel *subtitulo = new QLabel("UNIDAD AÉREA DE RESCATE  ·  OPERACIÓN CAVERNA", this);
    QFont fuenteSub("Consolas", 13, QFont::Bold);
    fuenteSub.setStyleHint(QFont::Monospace);
    fuenteSub.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    subtitulo->setFont(fuenteSub);
    subtitulo->setAlignment(Qt::AlignCenter);
    subtitulo->setStyleSheet("color: #c8d0b0;");

    // ----- Panel con los botones -------------------------------------------
    QFrame *panel = new QFrame(this);
    panel->setObjectName("panelMenu");
    panel->setStyleSheet(
        "#panelMenu {"
        "  background-color: rgba(20, 26, 16, 200);"
        "  border: 2px solid #6b7a3a;"
        "  border-radius: 14px;"
        "}");

    botonIniciar       = crearBoton(":/img/boton_iniciar.png", "INICIAR MISIÓN");
    botonInstrucciones = crearBoton(":/img/boton_instrucciones.png", "INSTRUCCIONES");
    botonSalir         = crearBoton(":/img/boton_salir.png", "SALIR");

    QVBoxLayout *layoutPanel = new QVBoxLayout(panel);
    layoutPanel->setContentsMargins(28, 24, 28, 24);
    layoutPanel->setSpacing(12);
    layoutPanel->addWidget(botonIniciar, 0, Qt::AlignHCenter);
    layoutPanel->addWidget(botonInstrucciones, 0, Qt::AlignHCenter);
    layoutPanel->addWidget(botonSalir, 0, Qt::AlignHCenter);

    // ----- Pie --------------------------------------------------------------
    QLabel *pie = new QLabel("Mantén ESPACIO para elevar el helicóptero", this);
    pie->setAlignment(Qt::AlignCenter);
    pie->setStyleSheet("color: rgba(230, 230, 210, 170); font-size: 12px;");

    // ----- Distribución general ---------------------------------------------
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 16);
    layout->setSpacing(6);
    layout->addStretch(2);
    layout->addWidget(iconoHeli);
    layout->addWidget(titulo);
    layout->addWidget(subtitulo);
    layout->addSpacing(28);
    layout->addWidget(panel, 0, Qt::AlignHCenter);
    layout->addStretch(3);
    layout->addWidget(pie);

    // ----- Conexiones: botón -> señal del menú -------------------------------
    connect(botonIniciar, &QPushButton::clicked,
            this, &MenuPrincipal::iniciarMisionSolicitado);
    connect(botonInstrucciones, &QPushButton::clicked,
            this, &MenuPrincipal::instruccionesSolicitadas);
    connect(botonSalir, &QPushButton::clicked,
            this, &MenuPrincipal::salirSolicitado);

    botonIniciar->setFocus();
}

QPushButton *MenuPrincipal::crearBoton(const QString &rutaImagen,
                                       const QString &textoRespaldo)
{
    // El padre se asigna al agregarlo al layout del panel.
    QPushButton *boton = new QPushButton();
    boton->setCursor(Qt::PointingHandCursor);
    boton->setAccessibleName(textoRespaldo);
    boton->setToolTip(textoRespaldo);

    QPixmap imagen(rutaImagen);
    if (!imagen.isNull()) {
        // Botón gráfico: la imagen ya incluye el texto del botón
        boton->setIcon(QIcon(imagen));
        boton->setIconSize(imagen.size());
        boton->setFixedSize(imagen.width() + 12, imagen.height() + 10);
        boton->setStyleSheet(
            "QPushButton { background: transparent; border: 2px solid transparent;"
            "              border-radius: 12px; }"
            "QPushButton:hover { border-color: #e8d27a;"
            "                    background: rgba(232, 210, 122, 35); }"
            "QPushButton:focus { border-color: #9fb05a; }"
            "QPushButton:pressed { background: rgba(0, 0, 0, 90); padding-top: 3px; }");
    } else {
        // Recurso no disponible: botón de texto con el mismo estilo militar
        boton->setText(textoRespaldo);
        boton->setFixedSize(350, 64);
        boton->setStyleSheet(
            "QPushButton { background: #2f3a24; color: #f0ecd0; font: bold 18px;"
            "              border: 2px solid #6b7a3a; border-radius: 10px; }"
            "QPushButton:hover { background: #3f4d2f; border-color: #e8d27a; }"
            "QPushButton:pressed { background: #1f2718; }");
    }
    return boton;
}

void MenuPrincipal::paintEvent(QPaintEvent *evento)
{
    Q_UNUSED(evento);
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    // 1) Imagen de fondo cubriendo todo el widget (recortando sobrantes)
    if (!imagenFondo.isNull()) {
        const QPixmap escalada = imagenFondo.scaled(size(), Qt::KeepAspectRatioByExpanding,
                                                   Qt::SmoothTransformation);
        const int dx = (escalada.width() - width()) / 2;
        const int dy = (escalada.height() - height()) / 2;
        p.drawPixmap(0, 0, escalada, dx, dy, width(), height());
    } else {
        p.fillRect(rect(), QColor(30, 38, 24));
    }

    // 2) Velo verde militar oscuro para dar legibilidad a título y botones
    QLinearGradient velo(0, 0, 0, height());
    velo.setColorAt(0.0, QColor(10, 14, 8, 150));
    velo.setColorAt(0.5, QColor(20, 28, 14, 120));
    velo.setColorAt(1.0, QColor(5, 7, 4, 220));
    p.fillRect(rect(), velo);

    // 3) Viñeta: bordes más oscuros, centro más claro
    QRadialGradient vineta(rect().center(), qMax(width(), height()) * 0.75);
    vineta.setColorAt(0.0, QColor(0, 0, 0, 0));
    vineta.setColorAt(1.0, QColor(0, 0, 0, 170));
    p.fillRect(rect(), vineta);
}
