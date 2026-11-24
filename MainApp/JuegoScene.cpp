#include "JuegoScene.h"
#include "Helicoptero.h"
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QFont>

const double LIMITE_TECHO = 95;
const double LIMITE_SUELO = 540;
const double MEDIA_ALTURA_HELI = 28;

JuegoScene::JuegoScene(QObject* padre) : QGraphicsScene(padre){
    helicoptero = nullptr;
    mensaje = nullptr;

    setSceneRect(0, 0, 1024, 600);

    QPixmap fondo(":/img/fondo_cueva.png");
    fondo = fondo.scaled(1024, 600);
    addPixmap(fondo);

    QPixmap imagenTecho(":/img/techo.png");
    QPixmap imagenSuelo(":/img/suelo.png");
    for(int x = 0; x < 1024; x = x + 290){
        QGraphicsPixmapItem* techo = addPixmap(imagenTecho);
        techo->setPos(x, -20);

        QGraphicsPixmapItem* suelo = addPixmap(imagenSuelo);
        suelo->setPos(x, 600 - imagenSuelo.height());
    }

    helicoptero = new Helicoptero();
    addItem(helicoptero);

    mensaje = new QGraphicsTextItem();
    mensaje->setDefaultTextColor(Qt::white);
    mensaje->setFont(QFont("Arial", 22, QFont::Bold));
    mensaje->setTextWidth(1024);
    mensaje->setPos(0, 230);
    addItem(mensaje);
}

void JuegoScene::reiniciar(){
    helicoptero->reiniciar();
}

void JuegoScene::actualizar(double deltaTime){
    helicoptero->actualizar(deltaTime);
}

bool JuegoScene::hayColision() const{
    double arriba = helicoptero->getY() - MEDIA_ALTURA_HELI;
    double abajo = helicoptero->getY() + MEDIA_ALTURA_HELI;

    if(arriba < LIMITE_TECHO){
        return true;
    }
    if(abajo > LIMITE_SUELO){
        return true;
    }
    return false;
}

void JuegoScene::mostrarMensaje(QString texto){
    mensaje->setHtml("<center>" + texto + "</center>");
    mensaje->show();
}

void JuegoScene::ocultarMensaje(){
    mensaje->hide();
}

Helicoptero* JuegoScene::getHelicoptero() const{
    return helicoptero;
}
