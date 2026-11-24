#ifndef JUEGOSCENE_H
#define JUEGOSCENE_H

#include <QGraphicsScene>

class Helicoptero;
class QGraphicsTextItem;

// Escena del juego: fondo, techo, suelo, helicóptero y un texto de mensajes.
class JuegoScene : public QGraphicsScene {
    Q_OBJECT

public:
    JuegoScene(QObject* padre = nullptr);

    void reiniciar();
    void actualizar(double deltaTime);
    bool hayColision() const;            // ¿Tocó el techo o el suelo?
    void mostrarMensaje(QString texto);
    void ocultarMensaje();
    Helicoptero* getHelicoptero() const;

private:
    // La escena es dueña de estos objetos y los borra sola al destruirse.
    Helicoptero* helicoptero;
    QGraphicsTextItem* mensaje;
};

#endif
