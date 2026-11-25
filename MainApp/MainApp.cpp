#include <QApplication>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>
#include "helicoptero.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);   // inicia Qt y el ciclo de eventos

    QGraphicsScene escena(0,0,1024,600);


    //FONDO
    QPixmap fondo(":/img/fondo.png");
    auto *itemFondo = escena.addPixmap(fondo.scaled(1024,600));
    itemFondo -> setZValue(0);

    //HELICOPTERO
    Helicoptero *heli = new Helicoptero();
    heli->setPos(150,250);
    heli->setZValue(1);
    escena.addItem(heli);

    //Vista de ventana
    QGraphicsView vista(&escena);
    vista.setFixedSize(1024,600);
    vista.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setFrameShape(QFrame::NoFrame);
    vista.setWindowTitle("Helicopter Rescue");
    vista.show();


    //Gameloop se actualiza 60 veces por segundo
    QElapsedTimer reloj;
    reloj.start();
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&](){
        double dt = reloj.restart() / 1000.0;
        heli -> actualizar(dt);
        if(heli->y() + heli->pixmap().height()>540){
            heli->aterrizar(540);
        }


    });
    timer.start(16);

    return app.exec();              // mantiene la ventana abierta hasta que se cierre
}
