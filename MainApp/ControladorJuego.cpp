#include "ControladorJuego.h"
#include "JuegoScene.h"
#include "JuegoView.h"
#include "Helicoptero.h"
#include <QTimer>

ControladorJuego::ControladorJuego(JuegoView *vistaJuego, QObject *padre) : QObject(padre)
{
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

ControladorJuego::~ControladorJuego()
{
    delete escena;      // Al borrar la escena, ella borra el helicóptero y lo demás
    escena = nullptr;
}

void ControladorJuego::iniciarPartida()
{
    timer->stop();
    enVuelo = false;
    terminada = false;
    escena->reiniciar();
    escena->mostrarMensaje("Presiona ESPACIO para despegar");
    vista->setFocus();
}

void ControladorJuego::alPresionarEspacio(bool presionada)
{
    if (terminada) {
        return;
    }

    // El primer ESPACIO arranca el vuelo
    if (enVuelo == false && presionada == true) {
        enVuelo = true;
        escena->ocultarMensaje();
        reloj.start();
        timer->start(16);   // ~60 veces por segundo
    }

    if (enVuelo) {
        Helicoptero *helicoptero = escena->getHelicoptero();
        helicoptero->setImpulso(presionada);
    }
}

void ControladorJuego::alPresionarEnter()
{
    if (terminada) {
        iniciarPartida();
    }
}

void ControladorJuego::alPresionarEscape()
{
    timer->stop();
    emit volverAlMenu();
}

void ControladorJuego::actualizar()
{
    // Delta time: segundos reales desde el tick anterior
    double deltaTime = reloj.restart() / 1000.0;

    // Evita saltos grandes si la ventana se congela
    if (deltaTime > 0.05) {
        deltaTime = 0.05;
    }

    escena->actualizar(deltaTime);

    if (escena->hayColision()) {
        terminarPartida();
    }
}

void ControladorJuego::terminarPartida()
{
    timer->stop();          // Detiene el movimiento
    enVuelo = false;
    terminada = true;
    escena->mostrarMensaje("MISIÓN TERMINADA<br>El helicóptero chocó<br><br>"
                           "ENTER: reintentar &nbsp;&nbsp; ESC: menú");
}
