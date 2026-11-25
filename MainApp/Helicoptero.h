#ifndef HELICOPTERO_H
#define HELICOPTERO_H
#include <QGraphicsPixmapItem>

class Helicoptero : public QGraphicsPixmapItem
{
public:
    Helicoptero();
    void actualizar(double dt);
    void aterrizar(double ySuelo);

private:
    double velocidadY = 0;
    double gravedad   = 900;
};

#endif // HELICOPTERO_H
