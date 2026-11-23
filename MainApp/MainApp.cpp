/*
 * MainApp.cpp
 * ---------------------------------------------------------------------------
 * Punto de entrada de HELICOPTER RESCUE.
 * Solo crea la aplicación Qt y la ventana principal; toda la lógica vive en
 * sus respectivas clases.
 */

#include "VentanaPrincipal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Helicopter Rescue");

    // La ventana se crea en la pila (sin new): se destruye automáticamente
    // al terminar main(), y con ella todos sus widgets hijos.
    VentanaPrincipal ventana;
    ventana.show();

    return app.exec();   // Ciclo de eventos: teclado, timers, dibujo...
}
