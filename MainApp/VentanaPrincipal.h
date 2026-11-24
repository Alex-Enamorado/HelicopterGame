#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>

class QStackedWidget;
class MenuPrincipal;
class JuegoView;
class ControladorJuego;

// Ventana de la aplicación. Cambia entre el menú y el juego.
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    VentanaPrincipal(QWidget* padre = nullptr);

private slots:
    void iniciarMision();
    void mostrarInstrucciones();
    void mostrarMenu();

private:
    // Todos tienen padre en Qt: se borran solos al cerrar la ventana
    QStackedWidget* pantallas;
    MenuPrincipal* menu;
    JuegoView* vista;
    ControladorJuego* controlador;
};

#endif
