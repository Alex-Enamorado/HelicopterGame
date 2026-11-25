#include "Helicoptero.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <algorithm>

using namespace std;

Helicoptero::Helicoptero(){
    QPixmap imagenHelicoptero(":/img/helicoptero.png");
    setPixmap(imagenHelicoptero.scaledToWidth(120, Qt::SmoothTransformation));

    setTransformOriginPoint(
        pixmap().width() / 2.0,
        pixmap().height() / 2.0
    );
}



void Helicoptero::actualizar(double segundosTranscurridos){
    double aceleracionY = gravedad;

    if(impulsoActivo){
        aceleracionY -= empuje;
    }

    if(abajoActivo){
        aceleracionY += gravedad;
    }

    velocidadY += aceleracionY * segundosTranscurridos;

    if(velocidadY > velocidadMaximaY){
        velocidadY = velocidadMaximaY;
    }

    if(velocidadY < -velocidadMaximaY){
        velocidadY = -velocidadMaximaY;
    }

    setY(y() + velocidadY * segundosTranscurridos);

    if(izquierdaActiva && !derechaActiva){
        velocidadX -= aceleracionX * segundosTranscurridos;
    } else if(derechaActiva && !izquierdaActiva){
        velocidadX += aceleracionX * segundosTranscurridos;
    } else{
        if(velocidadX > 0){
            velocidadX -= friccion * segundosTranscurridos;
            if(velocidadX < 0){
                velocidadX = 0;
            }
        } else if(velocidadX < 0){
            velocidadX += friccion * segundosTranscurridos;
            if (velocidadX > 0) {
                velocidadX = 0;
            }
        }
    }


    if(velocidadX > velocidadMaximaX){
        velocidadX = velocidadMaximaX;
    }

    if(velocidadX < -velocidadMaximaX){
        velocidadX = -velocidadMaximaX;
    }
    setX(x() + velocidadX * segundosTranscurridos);

    double anguloObjetivo = velocidadY / 25.0;

    anguloObjetivo = clamp(
        anguloObjetivo,
        -rotacionMaxima,
        rotacionMaxima
    );

    rotacionActual += (anguloObjetivo - rotacionActual) * min(1.0, 5.0 * segundosTranscurridos);
    setRotation(rotacionActual);

    if(scene() != nullptr){
        double anchoDeLaEscena = scene()->sceneRect().width();
        if(x() < 0){
            setX(0);
            velocidadX = 0;
        }

        if(x() + pixmap().width() > anchoDeLaEscena){
            setX(anchoDeLaEscena - pixmap().width());
            velocidadX = 0;
        }
    }
}


void Helicoptero::aterrizar(double alturaDelSuelo){
    setY(alturaDelSuelo - pixmap().height());
    velocidadY = 0;
}

void Helicoptero::setImpulso(bool estaActivo){
    impulsoActivo = estaActivo;
}

void Helicoptero::setAbajo(bool estaActivo){
    abajoActivo = estaActivo;
}

void Helicoptero::setIzquierda(bool estaActivo){
    izquierdaActiva = estaActivo;
}

void Helicoptero::setDerecha(bool estaActivo){
    derechaActiva = estaActivo;
}

