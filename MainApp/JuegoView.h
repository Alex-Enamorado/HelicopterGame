#ifndef JUEGOVIEW_H
#define JUEGOVIEW_H

#include <QGraphicsView>

class QKeyEvent;
class QFocusEvent;
class QResizeEvent;
class QShowEvent;

/*
 * JuegoView
 * ---------------------------------------------------------------------------
 * Ventana a través de la cual se observa la JuegoScene.
 *
 * Responsabilidades:
 *   - Mostrar la escena escalada al tamaño de la ventana, sin deformarla.
 *   - Recibir los eventos de teclado y traducirlos a SEÑALES con significado
 *     de juego (impulso, reintentar, volver al menú).
 *
 * No contiene lógica de física ni de reglas: solo avisa "qué pidió el
 * jugador". El ControladorJuego decide qué hacer con eso.
 */
class JuegoView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit JuegoView(QWidget *padre = nullptr);
    ~JuegoView() override = default;

    void ajustarEscena();   // Escala la escena para llenar la vista sin deformarla

signals:
    void impulsoCambiado(bool activo);   // ESPACIO presionado (true) / liberado (false)
    void reintentarSolicitado();         // ENTER o R
    void menuSolicitado();               // ESC

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;
    void focusOutEvent(QFocusEvent *evento) override;
    void resizeEvent(QResizeEvent *evento) override;
    void showEvent(QShowEvent *evento) override;
};

#endif // JUEGOVIEW_H
