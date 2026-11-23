#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>
#include <QPixmap>

class QPushButton;
class QPaintEvent;

/*
 * MenuPrincipal
 * ---------------------------------------------------------------------------
 * Pantalla del menú con el título HELICOPTER RESCUE y tres botones:
 * Iniciar Misión, Instrucciones y Salir.
 *
 * Solo se encarga de la INTERFAZ: cuando se presiona un botón emite una
 * señal y es la VentanaPrincipal quien decide qué hacer. Así el menú se
 * puede reutilizar o cambiar sin tocar la lógica del juego.
 */
class MenuPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPrincipal(QWidget *padre = nullptr);
    ~MenuPrincipal() override = default;

signals:
    void iniciarMisionSolicitado();
    void instruccionesSolicitadas();
    void salirSolicitado();

protected:
    void paintEvent(QPaintEvent *evento) override;   // Dibuja el fondo temático

private:
    QPushButton *crearBoton(const QString &rutaImagen, const QString &textoRespaldo);

    QPixmap imagenFondo;   // Se carga una sola vez (objeto por valor, sin new)

    // Punteros no propietarios: el layout los asigna como hijos del menú.
    QPushButton *botonIniciar       = nullptr;
    QPushButton *botonInstrucciones = nullptr;
    QPushButton *botonSalir         = nullptr;
};

#endif // MENUPRINCIPAL_H
