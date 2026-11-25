#include <QApplication>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QStackedWidget>
#include "helicoptero.h"
#include "MenuPrincipal.h"

class VistaJuego : public QGraphicsView{
    public:
        VistaJuego(QGraphicsScene *escenaDelJuego, Helicoptero *helicoptero)
            : QGraphicsView(escenaDelJuego), helicoptero(helicoptero){
            setFocusPolicy(Qt::StrongFocus);
        }

    protected:
        void keyPressEvent(QKeyEvent *eventoTeclado) override{
            if(eventoTeclado->isAutoRepeat()){ return; }
            actualizarEstadoTecla(eventoTeclado->key(), true);
        }

        void keyReleaseEvent(QKeyEvent *eventoTeclado) override{
            if(eventoTeclado->isAutoRepeat()){ return; }
            actualizarEstadoTecla(eventoTeclado->key(), false);
        }

        void focusOutEvent(QFocusEvent *eventoFoco) override{
            teclaEspacioPresionada = teclaFlechaArribaPresionada = false;
            helicoptero->setImpulso(false);
            helicoptero->setAbajo(false);
            helicoptero->setIzquierda(false);
            helicoptero->setDerecha(false);
            QGraphicsView::focusOutEvent(eventoFoco);
        }

    private:
        void actualizarEstadoTecla(int codigoTecla, bool estaPresionada){
            switch(codigoTecla){
                case Qt::Key_Space:
                    teclaEspacioPresionada = estaPresionada;
                    helicoptero->setImpulso(teclaEspacioPresionada || teclaFlechaArribaPresionada);
                    break;
                case Qt::Key_Up:
                    teclaFlechaArribaPresionada = estaPresionada;
                    helicoptero->setImpulso(teclaEspacioPresionada || teclaFlechaArribaPresionada);
                    break;
                case Qt::Key_Down:  helicoptero->setAbajo(estaPresionada); break;
                case Qt::Key_Left:  helicoptero->setIzquierda(estaPresionada); break;
                case Qt::Key_Right: helicoptero->setDerecha(estaPresionada); break;
            }
        }

        Helicoptero *helicoptero;
        bool teclaEspacioPresionada = false;
        bool teclaFlechaArribaPresionada = false;
};


int main(int argc, char *argv[]){
    QApplication aplicacion(argc, argv);

    QStackedWidget ventanaPrincipal;
    QGraphicsScene escenaDelJuego(0,0,1024,600);
    QPixmap imagenFondo(":/img/fondo.png");
    auto *itemImagenFondo = escenaDelJuego.addPixmap(imagenFondo.scaled(1024,600));
    itemImagenFondo -> setZValue(0);

    Helicoptero *helicoptero = new Helicoptero();
    helicoptero->setPos(150,250);
    helicoptero->setZValue(1);
    escenaDelJuego.addItem(helicoptero);

    VistaJuego vistaJuego(&escenaDelJuego, helicoptero);
    vistaJuego.setFixedSize(1024,600);
    vistaJuego.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaJuego.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaJuego.setFrameShape(QFrame::NoFrame);

    MenuPrincipal menuPrincipal;

    ventanaPrincipal.setFixedSize(1024,600);
    ventanaPrincipal.setWindowTitle("Helicopter Rescue");
    ventanaPrincipal.addWidget(&menuPrincipal);
    ventanaPrincipal.addWidget(&vistaJuego);
    ventanaPrincipal.setCurrentWidget(&menuPrincipal);
    ventanaPrincipal.show();

    QElapsedTimer relojEntreCuadros;
    QTimer temporizadorJuego;


    QObject::connect(&menuPrincipal, &MenuPrincipal::jugar, [&](){
        ventanaPrincipal.setCurrentWidget(&vistaJuego);
        vistaJuego.setFocus();
        relojEntreCuadros.start();
        temporizadorJuego.start(16);
    });

    QObject::connect(&temporizadorJuego, &QTimer::timeout, [&](){
        const double alturaDelSuelo = 540;
        double segundosTranscurridos = relojEntreCuadros.restart() / 1000.0;
        helicoptero -> actualizar(segundosTranscurridos);
        if(helicoptero->y() + helicoptero->pixmap().height() > alturaDelSuelo){
            helicoptero->aterrizar(alturaDelSuelo);
        }
    });
    return aplicacion.exec();
}
