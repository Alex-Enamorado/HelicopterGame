#ifndef JUEGOSCENE_H
#define JUEGOSCENE_H

#include <QGraphicsScene>

class Helicoptero;
class QGraphicsTextItem;

class JuegoScene : public QGraphicsScene {
    Q_OBJECT

public:
    JuegoScene(QObject* padre = nullptr);

    void reiniciar();
    void actualizar(double deltaTime);
    bool hayColision() const;
    void mostrarMensaje(QString texto);
    void ocultarMensaje();
    Helicoptero* getHelicoptero() const;

private:
    Helicoptero* helicoptero;
    QGraphicsTextItem* mensaje;
};

#endif
