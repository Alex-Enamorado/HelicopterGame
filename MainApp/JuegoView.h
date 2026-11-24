#ifndef JUEGOVIEW_H
#define JUEGOVIEW_H

#include <QGraphicsView>

class JuegoView : public QGraphicsView {
    Q_OBJECT

public:
    JuegoView(QWidget* padre = nullptr);

signals:
    void teclaEspacio(bool presionada);
    void teclaEnter();
    void teclaEscape();

protected:
    void keyPressEvent(QKeyEvent* evento) override;
    void keyReleaseEvent(QKeyEvent* evento) override;
};

#endif
