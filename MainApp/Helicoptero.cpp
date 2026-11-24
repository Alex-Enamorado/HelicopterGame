#include "Helicoptero.h"
#include <QPixmap>

using namespace std;

Helicoptero::Helicoptero(){
    x = 220;
    y = 300;
    velocidadY = 0;
    aceleracion = 0;
    gravedad = 900;
    empuje = 1800;
    rotacion = 0;
    impulso = false;

    QPixmap imagen(":/img/helicoptero.png");
    imagen = imagen.scaledToWidth(110, Qt::SmoothTransformation);
    setPixmap(imagen);
    setTransformationMode(Qt::SmoothTransformation);

    setOffset(-imagen.width() / 2.0, -imagen.height() / 2.0);

    reiniciar();
}

void Helicoptero::reiniciar(){
    y = 300;
    velocidadY = 0;
    aceleracion = 0;
    rotacion = 0;
    impulso = false;
    setPos(x, y);
    setRotation(rotacion);
}

void Helicoptero::setImpulso(bool activo){
    impulso = activo;
}

void Helicoptero::actualizar(double deltaTime){
    if(impulso){
        aceleracion = gravedad - empuje;
    } else {
        aceleracion = gravedad;
    }

    velocidadY = velocidadY + aceleracion * deltaTime;

    if(velocidadY > 450){
        velocidadY = 450;
    }
    if(velocidadY < -450){
        velocidadY = -450;
    }

    y = y + velocidadY * deltaTime;

    double anguloObjetivo = velocidadY / 25.0;
    rotacion = rotacion + (anguloObjetivo - rotacion) * 5.0 * deltaTime;

    setPos(x, y);
    setRotation(rotacion);
}

double Helicoptero::getY() const{
    return y;
}
