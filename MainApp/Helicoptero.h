#ifndef HELICOPTERO_H
#define HELICOPTERO_H
#include <QGraphicsPixmapItem>

class Helicoptero : public QGraphicsPixmapItem{
    public:
        Helicoptero();

        void actualizar(double dt);
        void aterrizar(double ySuelo);

        void setImpulso(bool activo);
        void setAbajo(bool activo);

        void setIzquierda(bool activo);
        void setDerecha(bool activo);

    private:
        double velocidadY = 0;
        double velocidadX = 0;

        double gravedad = 900;
        double empuje = 1800;
        double velocidadMaxY = 450;

        double aceleracionX = 1500;
        double friccion = 1200;
        double velocidadMaxX = 350;

        bool impulsoActivo = false;
        bool abajoActivo = false;
        bool izquierdaActiva = false;
        bool derechaActiva = false;


        double rotacion = 0;
        double rotacionMaxima = 18;
};

#endif
