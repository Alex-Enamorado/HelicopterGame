#include "ControladorJuego.h"
#include "JuegoScene.h"
#include "JuegoView.h"
#include "Helicoptero.h"
#include "Configuracion.h"

#include <QTimer>

ControladorJuego::ControladorJuego(JuegoView *vista, QObject *padre)
    : QObject(padre),
      vista(vista)
{
    // La escena se crea SIN padre a propósito: este controlador es su único
    // dueño y la libera explícitamente con delete en el destructor.
    escena = new JuegoScene();

    // El timer sí tiene padre (this): Qt lo liberará junto con el controlador.
    temporizador = new QTimer(this);
    temporizador->setInterval(Config::INTERVALO_TIMER_MS);
    temporizador->setTimerType(Qt::PreciseTimer);
    connect(temporizador, &QTimer::timeout, this, &ControladorJuego::actualizar);

    if (this->vista) {
        this->vista->setScene(escena);
        connect(this->vista, &JuegoView::impulsoCambiado,
                this, &ControladorJuego::alCambiarImpulso);
        connect(this->vista, &JuegoView::reintentarSolicitado,
                this, &ControladorJuego::alSolicitarReintento);
        connect(this->vista, &JuegoView::menuSolicitado,
                this, &ControladorJuego::alSolicitarMenu);
    }
}

ControladorJuego::~ControladorJuego()
{
    if (temporizador)
        temporizador->stop();

    // Antes de borrar la escena se desconecta de la vista, para que la vista
    // nunca apunte a una escena destruida (puntero colgante).
    // Gracias a QPointer, si la vista ya fue destruida aquí vale nullptr.
    if (vista && vista->scene() == escena)
        vista->setScene(nullptr);

    delete escena;      // new en el constructor  <->  delete aquí
    escena = nullptr;   // Evita usar la dirección liberada por error
}

// ---------------------------------------------------------------------------
// Control de la partida
// ---------------------------------------------------------------------------

void ControladorJuego::iniciarPartida()
{
    temporizador->stop();
    escena->reiniciar();
    estado = Estado::Preparado;

    // La física aún no corre: se espera a que el jugador presione ESPACIO.
    escena->mostrarMensaje("¿LISTO PARA DESPEGAR?",
                           "Mantén <b>ESPACIO</b> para elevar el helicóptero<br>"
                           "<span style='color:#b0b0a0;'>ESC: volver al menú</span>",
                           false);

    if (vista) {
        vista->ajustarEscena();
        vista->setFocus();
    }
}

void ControladorJuego::detenerPartida()
{
    temporizador->stop();
    if (escena->getHelicoptero())
        escena->getHelicoptero()->setImpulso(false);
    estado = Estado::Inactivo;
}

void ControladorJuego::terminarPartida(const QString &motivo)
{
    temporizador->stop();
    escena->getHelicoptero()->detener();
    estado = Estado::Terminada;

    escena->mostrarMensaje("MISIÓN TERMINADA",
                           QString("%1<br>Tiempo de vuelo: %2 s<br><br>"
                                   "<b>ENTER</b> o <b>R</b>: reintentar &nbsp;·&nbsp; "
                                   "<b>ESC</b>: menú principal")
                               .arg(motivo)
                               .arg(escena->getTiempoVuelo(), 0, 'f', 1),
                           true);
}

// ---------------------------------------------------------------------------
// Ciclo de juego
// ---------------------------------------------------------------------------

void ControladorJuego::actualizar()
{
    if (estado != Estado::EnCurso)
        return;

    // Delta time: segundos reales transcurridos desde el tick anterior.
    // El QTimer no es exacto (puede tardar 15, 17 o 30 ms), por eso se mide.
    double deltaTime = reloj.nsecsElapsed() / 1.0e9;
    reloj.restart();

    // Si la aplicación se congeló (por ejemplo al arrastrar la ventana), se
    // limita el paso para que el helicóptero no "atraviese" el suelo.
    if (deltaTime > Config::DELTA_TIME_MAXIMO)
        deltaTime = Config::DELTA_TIME_MAXIMO;

    // 1) Física, posición y rotación
    escena->actualizar(deltaTime);

    // 2) Colisiones
    switch (escena->verificarColisiones()) {
    case JuegoScene::Impacto::Techo:
        terminarPartida("El helicóptero impactó contra el <b>techo</b> de la cueva.");
        break;
    case JuegoScene::Impacto::Suelo:
        terminarPartida("El helicóptero impactó contra el <b>suelo</b>.");
        break;
    case JuegoScene::Impacto::Ninguno:
        break;
    }
}

// ---------------------------------------------------------------------------
// Respuestas al teclado (llegan como señales desde JuegoView)
// ---------------------------------------------------------------------------

void ControladorJuego::alCambiarImpulso(bool activo)
{
    Helicoptero *heli = escena->getHelicoptero();
    if (!heli)
        return;

    if (estado == Estado::Preparado && activo) {
        // Primer ESPACIO: comienza el vuelo
        estado = Estado::EnCurso;
        escena->ocultarMensaje();
        reloj.start();
        temporizador->start();
    }

    if (estado == Estado::EnCurso)
        heli->setImpulso(activo);
}

void ControladorJuego::alSolicitarReintento()
{
    if (estado == Estado::Terminada)
        iniciarPartida();
}

void ControladorJuego::alSolicitarMenu()
{
    detenerPartida();
    emit volverAlMenu();
}
