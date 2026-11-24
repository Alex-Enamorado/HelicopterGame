#include <QApplication>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);   // inicia Qt y el ciclo de eventos

    QGraphicsScene escena(0,0,1024,600);


    //FONDO
    QPixmap fondo(":/img/fondo.png");
    auto *itemFondo = escena.addPixmap(fondo.scaled(1024,600));
    itemFondo -> setZValue(0);

    //HELICOPTERO
    QPixmap heli(":/img/helicoptero.png");
    auto *itemHeli = escena.addPixmap(heli.scaledToWidth(120, Qt::SmoothTransformation));
    itemHeli -> setPos(150,250);
    itemHeli -> setZValue(1);

    //Vista de ventana
    QGraphicsView vista(&escena);
    vista.setFixedSize(1024,600);
    vista.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setFrameShape(QFrame::NoFrame);
    vista.setWindowTitle("Helicopter Rescue");
    vista.show();






    return app.exec();              // mantiene la ventana abierta hasta cerrarla
}
