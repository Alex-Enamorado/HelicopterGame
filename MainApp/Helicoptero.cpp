#include "Helicoptero.h"
#include "Configuracion.h"
#include "Sprites.h"

#include <QPixmap>
#include <QPainter>
#include <QtMath>

Helicoptero::Helicoptero(QGraphicsItem *padre)
    : QGraphicsPixmapItem(padre),
      posX(Config::HELI_POS_X_INICIAL),
      posY(Config::HELI_POS_Y_INICIAL),
      velocidadY(0.0),
      aceleracion(0.0),
      gravedad(Config::GRAVEDAD),
      empujeMotor(Config::EMPUJE_MOTOR),
      rotacionActual(0.0),
      impulsoActivo(false),
      detenido(false)
{
    cargarSprite();

    // La colisión usa la silueta real del dibujo (píxeles no transparentes),
    // no el rectángulo completo de la imagen.
    setShapeMode(QGraphicsPixmapItem::MaskShape);
    setTransformationMode(Qt::SmoothTransformation);
    setZValue(10);   // Por encima del fondo, suelo y techo

    aplicarPosicionGrafica();
}

void Helicoptero::cargarSprite()
{
    QPixmap imagen = Sprites::cargar(":/img/helicoptero.png");

    if (imagen.isNull()) {
        // Recurso no encontrado: se dibuja una figura temporal para que el
        // juego siga funcionando mientras se corrige el recurso.
        imagen = QPixmap(170, 105);
        imagen.fill(Qt::transparent);
        QPainter p(&imagen);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(85, 107, 47));          // Verde militar
        p.setPen(QPen(Qt::black, 3));
        p.drawEllipse(40, 30, 110, 55);           // Cabina
        p.drawRect(0, 50, 50, 12);                // Cola
        p.drawLine(20, 18, 160, 18);              // Rotor
        p.drawLine(60, 98, 140, 98);              // Patín
        p.end();
    }

    // Se escala una sola vez al tamaño deseado (más barato que escalar
    // en cada cuadro).
    imagen = imagen.scaledToWidth(static_cast<int>(Config::HELI_ANCHO),
                                  Qt::SmoothTransformation);
    setPixmap(imagen);

    // El origen del item queda en el centro de la imagen. Así:
    //   - pos() representa el centro del helicóptero.
    //   - la rotación gira alrededor del centro y no de una esquina.
    setOffset(-imagen.width() / 2.0, -imagen.height() / 2.0);
}

void Helicoptero::setImpulso(bool activo)
{
    impulsoActivo = activo;
}

void Helicoptero::actualizar(double deltaTime)
{
    if (detenido || deltaTime <= 0.0)
        return;

    // 1) Aceleración neta. En Qt el eje Y crece hacia ABAJO, por eso:
    //    gravedad   -> positiva (hacia abajo)
    //    empuje     -> negativo (hacia arriba)
    aceleracion = gravedad;
    if (impulsoActivo)
        aceleracion -= empujeMotor;

    // 2) Velocidad:  vy = vy + aceleracion * deltaTime
    velocidadY = velocidadY + aceleracion * deltaTime;

    // Límite de velocidad para que el control sea manejable
    velocidadY = qBound(-Config::VELOCIDAD_MAX_SUBIDA, velocidadY,
                        Config::VELOCIDAD_MAX_CAIDA);

    // 3) Posición:   y = y + vy * deltaTime
    posY = posY + velocidadY * deltaTime;

    actualizarRotacion(deltaTime);
    aplicarPosicionGrafica();
}

void Helicoptero::actualizarRotacion(double deltaTime)
{
    // Ángulo objetivo proporcional a la velocidad:
    //   subiendo (vy < 0) -> ángulo negativo -> nariz hacia arriba
    //   bajando  (vy > 0) -> ángulo positivo -> nariz hacia abajo
    double proporcion;
    if (velocidadY < 0.0)
        proporcion = velocidadY / Config::VELOCIDAD_MAX_SUBIDA;
    else
        proporcion = velocidadY / Config::VELOCIDAD_MAX_CAIDA;

    const double anguloObjetivo = proporcion * Config::ANGULO_MAXIMO;

    // Interpolación: en lugar de saltar al ángulo objetivo, se avanza solo
    // una fracción de la diferencia en cada cuadro. Eso evita giros bruscos.
    // La fracción depende de deltaTime para ser independiente de los FPS.
    double factor = Config::SUAVIZADO_ROTACION * deltaTime;
    if (factor > 1.0)
        factor = 1.0;

    rotacionActual += (anguloObjetivo - rotacionActual) * factor;
}

void Helicoptero::aplicarPosicionGrafica()
{
    setPos(posX, posY);
    setRotation(rotacionActual);
}

void Helicoptero::detener()
{
    detenido = true;
    impulsoActivo = false;
    velocidadY = 0.0;
    aceleracion = 0.0;
}

void Helicoptero::reiniciar(const QPointF &posicionInicial)
{
    posX = posicionInicial.x();
    posY = posicionInicial.y();
    velocidadY = 0.0;
    aceleracion = 0.0;
    rotacionActual = 0.0;
    impulsoActivo = false;
    detenido = false;
    aplicarPosicionGrafica();
}
