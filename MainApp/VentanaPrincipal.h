#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>

class QStackedWidget;
class MenuPrincipal;
class JuegoView;
class ControladorJuego;

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    VentanaPrincipal(QWidget* padre = nullptr);

private slots:
    void iniciarMision();
    void mostrarInstrucciones();
    void mostrarMenu();

private:
    QStackedWidget* pantallas;
    MenuPrincipal* menu;
    JuegoView* vista;
    ControladorJuego* controlador;
};

#endif
