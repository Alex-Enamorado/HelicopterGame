#include "Helicoptero.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <algorithm>

using namespace std;

Helicoptero::Helicoptero(){
    QPixmap img(":/img/helicoptero.png");
    setPixmap(img.scaledToWidth(120, Qt::SmoothTransformation));

    setTransformOriginPoint(
        pixmap().width() / 2.0,
        pixmap().height() / 2.0
    );
}



void Helicoptero::actualizar(double dt){
    double aceleracionY = gravedad;

    if(impulsoActivo){
        aceleracionY -= empuje;
    }

    if(abajoActivo){
        aceleracionY += gravedad;
    }

    velocidadY += aceleracionY * dt;

    if(velocidadY > velocidadMaxY){
        velocidadY = velocidadMaxY;
    }

    if(velocidadY < -velocidadMaxY){
        velocidadY = -velocidadMaxY;
    }

    setY(y() + velocidadY * dt);

    if(izquierdaActiva && !derechaActiva){
        velocidadX -= aceleracionX * dt;
    } else if(derechaActiva && !izquierdaActiva){
        velocidadX += aceleracionX * dt;
    } else{
        if(velocidadX > 0){
            velocidadX -= friccion * dt;
            if(velocidadX < 0){
                velocidadX = 0;
            }
        } else if(velocidadX < 0){
            velocidadX += friccion * dt;
            if(velocidadX > 0){
                velocidadX = 0;
            }
        }
    }


    if(velocidadX > velocidadMaxX){
        velocidadX = velocidadMaxX;
    }

    if(velocidadX < -velocidadMaxX){
        velocidadX = -velocidadMaxX;
    }
    setX(x() + velocidadX * dt);

    double anguloObjetivo = velocidadY / 25.0;

    anguloObjetivo = clamp(
        anguloObjetivo,
        -rotacionMaxima,
        rotacionMaxima
    );

    rotacion += (anguloObjetivo - rotacion) * min(1.0, 5.0 * dt);
    setRotation(rotacion);

    if(scene() != nullptr){
        double anchoEscena = scene()->sceneRect().width();
        if(x() < 0){
            setX(0);
            velocidadX = 0;
        }

        if(x() + pixmap().width() > anchoEscena){
            setX(anchoEscena - pixmap().width());
            velocidadX = 0;
        }
    }
}


void Helicoptero::aterrizar(double ySuelo){
    setY(ySuelo - pixmap().height());
    velocidadY = 0;
}

void Helicoptero::setImpulso(bool activo){
    impulsoActivo = activo;
}

void Helicoptero::setAbajo(bool activo){
    abajoActivo = activo;
}

void Helicoptero::setIzquierda(bool activo){
    izquierdaActiva = activo;
}

void Helicoptero::setDerecha(bool activo){
    derechaActiva = activo;
}

