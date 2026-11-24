#include "ControladorJuego.h"
#include "JuegoScene.h"
#include "JuegoView.h"
#include "Helicoptero.h"
#include <QTimer>

using namespace std;

ControladorJuego::ControladorJuego(JuegoView* vistaJuego, QObject* padre) : QObject(padre){
    vista = vistaJuego;
    enVuelo = false;
    terminada = false;

    escena = new JuegoScene();
    vista->setScene(escena);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ControladorJuego::actualizar);

    connect(vista, &JuegoView::teclaEspacio, this, &ControladorJuego::alPresionarEspacio);
    connect(vista, &JuegoView::teclaEnter, this, &ControladorJuego::alPresionarEnter);
    connect(vista, &JuegoView::teclaEscape, this, &ControladorJuego::alPresionarEscape);
}

ControladorJuego::~ControladorJuego(){
    delete escena;
    escena = nullptr;
}

void ControladorJuego::iniciarPartida(){
    timer->stop();
    enVuelo = false;
    terminada = false;
    escena->reiniciar();
    escena->mostrarMensaje("Presiona ESPACIO para despegar");
    vista->setFocus();
}

void ControladorJuego::alPresionarEspacio(bool presionada){
    if(terminada){
        return;
    }

    if(enVuelo == false && presionada == true){
        enVuelo = true;
        escena->ocultarMensaje();
        reloj.start();
        timer->start(16);
    }

    if(enVuelo){
        Helicoptero* helicoptero = escena->getHelicoptero();
        helicoptero->setImpulso(presionada);
    }
}

void ControladorJuego::alPresionarEnter(){
    if(terminada){
        iniciarPartida();
    }
}

void ControladorJuego::alPresionarEscape(){
    timer->stop();
    emit volverAlMenu();
}

void ControladorJuego::actualizar(){
    double deltaTime = reloj.restart() / 1000.0;

    if(deltaTime > 0.05){
        deltaTime = 0.05;
    }

    escena->actualizar(deltaTime);

    if(escena->hayColision()){
        terminarPartida();
    }
}

void ControladorJuego::terminarPartida(){
    timer->stop();
    enVuelo = false;
    terminada = true;
    escena->mostrarMensaje("MISIÓN TERMINADA<br>El helicóptero chocó<br><br>"
                           "ENTER: reintentar &nbsp;&nbsp; ESC: menú");
}
