#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>

class QStackedWidget;
class MenuPrincipal;
class JuegoView;
class ControladorJuego;

/*
 * VentanaPrincipal
 * ---------------------------------------------------------------------------
 * Ventana de la aplicación. Contiene un QStackedWidget con dos "pantallas":
 *   índice 0 -> MenuPrincipal
 *   índice 1 -> JuegoView
 * y cambia entre ellas según lo que pida el usuario.
 *
 * Propiedad de memoria (árbol padre-hijo de Qt):
 *   VentanaPrincipal
 *     ├── QStackedWidget (widget central)
 *     │     ├── MenuPrincipal
 *     │     └── JuegoView
 *     └── ControladorJuego
 *           ├── QTimer
 *           └── JuegoScene (liberada manualmente por el controlador)
 * Al cerrarse la ventana, Qt destruye todo el árbol automáticamente.
 */
class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *padre = nullptr);
    ~VentanaPrincipal() override = default;

private slots:
    void mostrarMenu();
    void iniciarMision();
    void mostrarInstrucciones();

private:
    // Punteros no propietarios (Qt los libera mediante el árbol de padres)
    QStackedWidget   *pantallas   = nullptr;
    MenuPrincipal    *menu        = nullptr;
    JuegoView        *vistaJuego  = nullptr;
    ControladorJuego *controlador = nullptr;
};

#endif // VENTANAPRINCIPAL_H
