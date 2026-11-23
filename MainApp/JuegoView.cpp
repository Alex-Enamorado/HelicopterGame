#include "JuegoView.h"

#include <QKeyEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QShowEvent>

JuegoView::JuegoView(QWidget *padre)
    : QGraphicsView(padre)
{
    // Sin barras de desplazamiento: la escena siempre se ajusta a la vista
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setBackgroundBrush(Qt::black);

    // Calidad de dibujo: suavizado de imágenes al escalar/rotar
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Redibujar todo en cada cuadro: más simple y sin "rastros" al rotar
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Necesario para recibir eventos de teclado
    setFocusPolicy(Qt::StrongFocus);
}

void JuegoView::keyPressEvent(QKeyEvent *evento)
{
    // Al mantener una tecla, el sistema operativo genera repeticiones
    // automáticas. Solo interesa el primer "presionar".
    if (evento->isAutoRepeat()) {
        evento->accept();
        return;
    }

    switch (evento->key()) {
    case Qt::Key_Space:
        emit impulsoCambiado(true);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_R:
        emit reintentarSolicitado();
        break;
    case Qt::Key_Escape:
        emit menuSolicitado();
        break;
    default:
        // Teclas no usadas: comportamiento normal de Qt
        QGraphicsView::keyPressEvent(evento);
        return;
    }
    evento->accept();
}

void JuegoView::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) {
        evento->accept();
        return;
    }

    if (evento->key() == Qt::Key_Space) {
        emit impulsoCambiado(false);
        evento->accept();
        return;
    }
    QGraphicsView::keyReleaseEvent(evento);
}

void JuegoView::focusOutEvent(QFocusEvent *evento)
{
    // Si la ventana pierde el foco mientras ESPACIO está presionado, nunca
    // llegaría el evento de "soltar". Se suelta el impulso por seguridad.
    emit impulsoCambiado(false);
    QGraphicsView::focusOutEvent(evento);
}

void JuegoView::resizeEvent(QResizeEvent *evento)
{
    QGraphicsView::resizeEvent(evento);
    ajustarEscena();
}

void JuegoView::showEvent(QShowEvent *evento)
{
    QGraphicsView::showEvent(evento);
    ajustarEscena();
}

void JuegoView::ajustarEscena()
{
    if (scene())
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}
