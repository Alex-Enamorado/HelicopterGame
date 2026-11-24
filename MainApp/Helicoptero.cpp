#include "Helicoptero.h"
#include <QPixmap>

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

    // Se mueve el dibujo para que el punto (0,0) quede en su centro:
    // así la posición es el centro y la rotación gira sobre el centro.
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
    // En Qt el eje Y crece hacia abajo: la gravedad es positiva
    // y el empuje del motor se resta.
    if(impulso){
        aceleracion = gravedad - empuje;
    } else {
        aceleracion = gravedad;
    }

    // Velocidad: vy = vy + aceleracion * deltaTime
    velocidadY = velocidadY + aceleracion * deltaTime;

    // Límite de velocidad para que se pueda controlar
    if(velocidadY > 450){
        velocidadY = 450;
    }
    if(velocidadY < -450){
        velocidadY = -450;
    }

    // Posición: y = y + vy * deltaTime
    y = y + velocidadY * deltaTime;

    // Inclinación: subir = nariz arriba (ángulo negativo), bajar = nariz abajo.
    // El ángulo se acerca poco a poco al objetivo para que no gire de golpe.
    double anguloObjetivo = velocidadY / 25.0;   // Máximo 450 / 25 = 18 grados
    rotacion = rotacion + (anguloObjetivo - rotacion) * 5.0 * deltaTime;

    setPos(x, y);
    setRotation(rotacion);
}

double Helicoptero::getY() const{
    return y;
}
