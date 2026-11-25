#include <QApplication>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include "helicoptero.h"

class VistaJuego : public QGraphicsView{
    public:
        VistaJuego(QGraphicsScene *escena, Helicoptero *heli)
            : QGraphicsView(escena), heli(heli){
            setFocusPolicy(Qt::StrongFocus);
        }

    protected:
        void keyPressEvent(QKeyEvent *e) override{
            if(e->isAutoRepeat()){ return; }
            cambiarTecla(e->key(), true);
        }

        void keyReleaseEvent(QKeyEvent *e) override{
            if(e->isAutoRepeat()){ return; }
            cambiarTecla(e->key(), false);
        }

        void focusOutEvent(QFocusEvent *e) override{
            espacio = arriba = false;
            heli->setImpulso(false);
            heli->setAbajo(false);
            heli->setIzquierda(false);
            heli->setDerecha(false);
            QGraphicsView::focusOutEvent(e);
        }

    private:
        void cambiarTecla(int tecla, bool presionada){
            switch(tecla){
                case Qt::Key_Space: espacio = presionada; heli->setImpulso(espacio || arriba); break;
                case Qt::Key_Up:    arriba = presionada;  heli->setImpulso(espacio || arriba); break;
                case Qt::Key_Down:  heli->setAbajo(presionada); break;
                case Qt::Key_Left:  heli->setIzquierda(presionada); break;
                case Qt::Key_Right: heli->setDerecha(presionada); break;
            }
        }

        Helicoptero *heli;
        bool espacio = false;
        bool arriba = false;
};


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QGraphicsScene escena(0,0,1024,600);
    QPixmap fondo(":/img/fondo.png");
    auto *itemFondo = escena.addPixmap(fondo.scaled(1024,600));
    itemFondo -> setZValue(0);

    Helicoptero *heli = new Helicoptero();
    heli->setPos(150,250);
    heli->setZValue(1);
    escena.addItem(heli);

    VistaJuego vista(&escena, heli);
    vista.setFixedSize(1024,600);
    vista.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista.setFrameShape(QFrame::NoFrame);
    vista.setWindowTitle("Helicopter Rescue");
    vista.show();
    vista.setFocus();

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
    return app.exec();
}
