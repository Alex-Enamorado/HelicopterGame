#ifndef JUEGOVIEW_H
#define JUEGOVIEW_H

#include <QGraphicsView>

// Muestra la escena y avisa (con señales) qué teclas presiona el jugador.
class JuegoView : public QGraphicsView
{
    Q_OBJECT

public:
    JuegoView(QWidget *padre = nullptr);

signals:
    void teclaEspacio(bool presionada);
    void teclaEnter();
    void teclaEscape();

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;
};

#endif
