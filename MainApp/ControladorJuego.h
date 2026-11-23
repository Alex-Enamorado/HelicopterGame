#ifndef CONTROLADORJUEGO_H
#define CONTROLADORJUEGO_H

#include <QObject>
#include <QPointer>
#include <QElapsedTimer>

class QTimer;
class JuegoScene;
class JuegoView;

/*
 * ControladorJuego
 * ---------------------------------------------------------------------------
 * Coordina una partida. Es el "director" que une las piezas:
 *
 *   JuegoView  --(señales de teclado)-->  ControladorJuego
 *   ControladorJuego --(actualizar / verificar)--> JuegoScene --> Helicoptero
 *
 * Responsabilidades:
 *   - Crear y destruir la JuegoScene.
 *   - Manejar el QTimer del ciclo de juego y calcular el delta time.
 *   - Llevar el estado de la partida (preparado, en curso, terminada).
 *   - Avisar a la ventana principal cuando el jugador quiere volver al menú.
 */
class ControladorJuego : public QObject
{
    Q_OBJECT

public:
    enum class Estado { Inactivo, Preparado, EnCurso, Terminada };

    explicit ControladorJuego(JuegoView *vista, QObject *padre = nullptr);
    ~ControladorJuego() override;

    Estado getEstado() const { return estado; }

public slots:
    void iniciarPartida();     // Prepara una partida nueva y arranca el timer
    void detenerPartida();     // Detiene el timer (al volver al menú)

signals:
    void volverAlMenu();

private slots:
    void actualizar();                    // Un "tick" del juego (lo llama el QTimer)
    void alCambiarImpulso(bool activo);
    void alSolicitarReintento();
    void alSolicitarMenu();

private:
    void terminarPartida(const QString &motivo);

    QPointer<JuegoView> vista;      // No propietario. QPointer se vuelve nullptr
                                    // solo si la vista se destruye antes.
    JuegoScene *escena = nullptr;   // Propietario: se crea con new y se libera
                                    // con delete en el destructor.
    QTimer *temporizador = nullptr; // Hijo de este objeto (Qt lo libera).
    QElapsedTimer reloj;            // Mide el tiempo real entre ticks.
    Estado estado = Estado::Inactivo;
};

#endif // CONTROLADORJUEGO_H
