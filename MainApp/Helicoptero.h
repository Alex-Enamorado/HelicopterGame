#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsPixmapItem>

// Helicóptero del jugador: guarda su física y se dibuja en la escena.
class Helicoptero : public QGraphicsPixmapItem
{
public:
    Helicoptero();

    void reiniciar();                    // Vuelve a la posición inicial
    void setImpulso(bool activo);        // true mientras se mantiene ESPACIO
    void actualizar(double deltaTime);   // Aplica la física un paso
    double getY() const;

private:
    double x;             // Posición horizontal
    double y;             // Posición vertical
    double velocidadY;    // Velocidad vertical (px/s)
    double aceleracion;   // Aceleración actual (px/s^2)
    double gravedad;      // Empuja hacia abajo
    double empuje;        // Empuja hacia arriba con ESPACIO
    double rotacion;      // Inclinación en grados
    bool impulso;         // ¿Está presionado ESPACIO?
};

#endif
