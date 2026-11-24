#include "VentanaPrincipal.h"
#include "MenuPrincipal.h"
#include "JuegoView.h"
#include "ControladorJuego.h"
#include <QStackedWidget>
#include <QMessageBox>

using namespace std;

VentanaPrincipal::VentanaPrincipal(QWidget* padre) : QMainWindow(padre){
    setWindowTitle("Helicopter Rescue");
    setFixedSize(1024, 600);

    pantallas = new QStackedWidget(this);
    setCentralWidget(pantallas);

    menu = new MenuPrincipal();
    vista = new JuegoView();
    pantallas->addWidget(menu);
    pantallas->addWidget(vista);

    controlador = new ControladorJuego(vista, this);

    connect(menu, &MenuPrincipal::iniciarPresionado, this, &VentanaPrincipal::iniciarMision);
    connect(menu, &MenuPrincipal::instruccionesPresionado, this, &VentanaPrincipal::mostrarInstrucciones);
    connect(menu, &MenuPrincipal::salirPresionado, this, &VentanaPrincipal::close);
    connect(controlador, &ControladorJuego::volverAlMenu, this, &VentanaPrincipal::mostrarMenu);
}

void VentanaPrincipal::iniciarMision(){
    pantallas->setCurrentWidget(vista);
    controlador->iniciarPartida();
}

void VentanaPrincipal::mostrarInstrucciones(){
    QMessageBox::information(this, "Instrucciones",
        "Mantén ESPACIO para subir.\n"
        "Suelta ESPACIO para bajar.\n"
        "No toques el techo ni el suelo.\n\n"
        "ENTER: reintentar\n"
        "ESC: volver al menú");
}

void VentanaPrincipal::mostrarMenu(){
    pantallas->setCurrentWidget(menu);
}
