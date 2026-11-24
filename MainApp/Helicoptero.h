#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsPixmapItem>

class Helicoptero : public QGraphicsPixmapItem {
public:
    Helicoptero();

    void reiniciar();
    void setImpulso(bool activo);
    void actualizar(double deltaTime);
    double getY() const;

private:
    double x;
    double y;
    double velocidadY;
    double aceleracion;
    double gravedad;
    double empuje;
    double rotacion;
    bool impulso;
};

#endif
