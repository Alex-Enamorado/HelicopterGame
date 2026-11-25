#ifndef HELICOPTERO_H
#define HELICOPTERO_H
#include <QGraphicsPixmapItem>

class Helicoptero : public QGraphicsPixmapItem{
    public:
        Helicoptero();

        void actualizar(double segundosTranscurridos);
        void aterrizar(double alturaDelSuelo);

        void setImpulso(bool estaActivo);
        void setAbajo(bool estaActivo);

        void setIzquierda(bool estaActivo);
        void setDerecha(bool estaActivo);

    private:
        double velocidadY = 0;
        double velocidadX = 0;

        double gravedad = 900;
        double empuje = 1800;
        double velocidadMaximaY = 450;

        double aceleracionX = 1500;
        double friccion = 1200;
        double velocidadMaximaX = 350;

        bool impulsoActivo = false;
        bool abajoActivo = false;
        bool izquierdaActiva = false;
        bool derechaActiva = false;


        double rotacionActual = 0;
        double rotacionMaxima = 18;
};

#endif
