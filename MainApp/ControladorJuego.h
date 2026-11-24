#ifndef CONTROLADORJUEGO_H
#define CONTROLADORJUEGO_H

#include <QObject>
#include <QElapsedTimer>

class QTimer;
class JuegoScene;
class JuegoView;

class ControladorJuego : public QObject {
    Q_OBJECT

public:
    ControladorJuego(JuegoView* vistaJuego, QObject* padre = nullptr);
    ~ControladorJuego();

    void iniciarPartida();

signals:
    void volverAlMenu();

private slots:
    void actualizar();
    void alPresionarEspacio(bool presionada);
    void alPresionarEnter();
    void alPresionarEscape();

private:
    void terminarPartida();

    JuegoView* vista;
    JuegoScene* escena;
    QTimer* timer;
    QElapsedTimer reloj;
    bool enVuelo;
    bool terminada;
};

#endif
