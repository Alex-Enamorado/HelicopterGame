#include "VentanaPrincipal.h"
#include "MenuPrincipal.h"
#include "JuegoView.h"
#include "ControladorJuego.h"

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>

VentanaPrincipal::VentanaPrincipal(QWidget *padre)
    : QMainWindow(padre)
{
    setWindowTitle("Helicopter Rescue");
    setWindowIcon(QIcon(":/img/helicoptero.png"));
    setMinimumSize(800, 500);
    resize(1100, 680);

    // Contenedor de pantallas. setCentralWidget lo hace hijo de la ventana.
    pantallas = new QStackedWidget(this);
    setCentralWidget(pantallas);

    // addWidget convierte al menú y a la vista en hijos del QStackedWidget.
    menu = new MenuPrincipal();
    vistaJuego = new JuegoView();
    pantallas->addWidget(menu);        // índice 0
    pantallas->addWidget(vistaJuego);  // índice 1

    // El controlador es hijo de la ventana: vive mientras la ventana viva.
    controlador = new ControladorJuego(vistaJuego, this);

    // ----- Conexiones -------------------------------------------------------
    connect(menu, &MenuPrincipal::iniciarMisionSolicitado,
            this, &VentanaPrincipal::iniciarMision);
    connect(menu, &MenuPrincipal::instruccionesSolicitadas,
            this, &VentanaPrincipal::mostrarInstrucciones);
    // close() cierra la ventana principal; al no quedar ventanas abiertas,
    // QApplication termina el ciclo de eventos y la aplicación sale.
    connect(menu, &MenuPrincipal::salirSolicitado,
            this, &VentanaPrincipal::close);

    connect(controlador, &ControladorJuego::volverAlMenu,
            this, &VentanaPrincipal::mostrarMenu);

    mostrarMenu();
}

void VentanaPrincipal::mostrarMenu()
{
    pantallas->setCurrentWidget(menu);
    menu->setFocus();
}

void VentanaPrincipal::iniciarMision()
{
    pantallas->setCurrentWidget(vistaJuego);
    controlador->iniciarPartida();
}

void VentanaPrincipal::mostrarInstrucciones()
{
    QMessageBox caja(this);
    caja.setWindowTitle("Instrucciones");
    caja.setIconPixmap(QPixmap(":/img/helicoptero.png")
                           .scaledToWidth(90, Qt::SmoothTransformation));
    caja.setTextFormat(Qt::RichText);
    caja.setText(
        "<h2 style='color:#e8d27a;'>CONTROLES DEL HELICÓPTERO</h2>"
        "<table cellspacing='6'>"
        "<tr><td><b>ESPACIO (mantener)</b></td><td>Encender el rotor y elevarse</td></tr>"
        "<tr><td><b>ESPACIO (soltar)</b></td><td>La gravedad hace descender</td></tr>"
        "<tr><td><b>ENTER / R</b></td><td>Reintentar tras terminar la misión</td></tr>"
        "<tr><td><b>ESC</b></td><td>Volver al menú principal</td></tr>"
        "</table>"
        "<p><b>Objetivo:</b> mantener el vuelo dentro de la cueva sin tocar "
        "el <b>techo</b> ni el <b>suelo</b>. Cualquier impacto termina la misión.</p>"
        "<p>Consejo: usa toques cortos de ESPACIO para estabilizar la altura.</p>");
    caja.setStandardButtons(QMessageBox::Ok);
    caja.button(QMessageBox::Ok)->setText("Entendido");
    caja.setStyleSheet(
        "QMessageBox { background-color: #1e2618; }"
        "QLabel { color: #eae6cc; font-size: 13px; }"
        "QPushButton { background: #3f4d2f; color: #f0ecd0; padding: 6px 18px;"
        "              border: 2px solid #6b7a3a; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { border-color: #e8d27a; }");
    caja.exec();   // Objeto en la pila (stack): se libera solo al salir de la función
}
