#ifndef CONTROLADORJUEGO_H
#define CONTROLADORJUEGO_H

#include <QObject>
#include <QElapsedTimer>

class QTimer;
class JuegoScene;
class JuegoView;

// Coordina la partida: recibe las teclas, mueve el juego con un QTimer
// y decide cuándo termina la misión.
class ControladorJuego : public QObject {
    Q_OBJECT

public:
    ControladorJuego(JuegoView* vistaJuego, QObject* padre = nullptr);
    ~ControladorJuego();

    void iniciarPartida();

signals:
    void volverAlMenu();

private slots:
    void actualizar();                         // Se llama en cada tick del QTimer
    void alPresionarEspacio(bool presionada);
    void alPresionarEnter();
    void alPresionarEscape();

private:
    void terminarPartida();

    JuegoView* vista;       // No es dueño (la ventana la borra)
    JuegoScene* escena;     // Es dueño: new en el constructor, delete en el destructor
    QTimer* timer;          // Hijo de este objeto: Qt lo borra solo
    QElapsedTimer reloj;    // Mide el tiempo real entre ticks (delta time)
    bool enVuelo;
    bool terminada;
};

#endif
