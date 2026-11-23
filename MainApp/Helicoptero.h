#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsPixmapItem>
#include <QPointF>

/*
 * Helicoptero
 * ---------------------------------------------------------------------------
 * Objeto del juego que representa al helicóptero del jugador.
 *
 * Responsabilidades:
 *   - Guardar su estado físico: posición (x, y), velocidad vertical,
 *     aceleración y gravedad.
 *   - Integrar la física usando delta time (tiempo transcurrido entre
 *     actualizaciones), de modo que se mueva igual en cualquier computadora.
 *   - Calcular una inclinación gráfica suave según su velocidad.
 *
 * NO sabe nada del teclado ni de la ventana: solo recibe la orden
 * "impulso activo / inactivo" desde el controlador. Así la lógica queda
 * separada de la interfaz.
 *
 * Hereda de QGraphicsPixmapItem para poder dibujarse dentro de una
 * QGraphicsScene. Cuando se agrega a la escena, la escena pasa a ser su
 * dueña y se encarga de liberarlo (no se debe hacer delete manual).
 */
class Helicoptero : public QGraphicsPixmapItem
{
public:
    explicit Helicoptero(QGraphicsItem *padre = nullptr);
    ~Helicoptero() override = default;

    // --- Control ------------------------------------------------------------
    void setImpulso(bool activo);          // ESPACIO presionado / liberado
    bool tieneImpulso() const { return impulsoActivo; }

    // --- Ciclo de vida -------------------------------------------------------
    void actualizar(double deltaTime);     // Avanza la física un paso
    void detener();                        // Congela el movimiento (impacto)
    void reiniciar(const QPointF &posicionInicial);

    // --- Consultas (útiles para HUD, colisiones y pruebas) -------------------
    double getPosX() const { return posX; }
    double getPosY() const { return posY; }
    double getVelocidadY() const { return velocidadY; }
    double getAceleracion() const { return aceleracion; }
    double getGravedad() const { return gravedad; }
    double getRotacion() const { return rotacionActual; }
    bool   estaDetenido() const { return detenido; }

private:
    void cargarSprite();                       // Carga la imagen o crea una temporal
    void actualizarRotacion(double deltaTime); // Inclinación suave
    void aplicarPosicionGrafica();             // Copia el estado físico al item

    // Estado físico (unidades: px, px/s, px/s^2)
    double posX;
    double posY;
    double velocidadY;
    double aceleracion;     // Aceleración neta calculada en cada paso
    double gravedad;        // Siempre empuja hacia abajo (+y)
    double empujeMotor;     // Empuja hacia arriba (-y) mientras hay impulso

    // Estado gráfico
    double rotacionActual;  // Grados; negativo = nariz hacia arriba

    // Estado de control
    bool impulsoActivo;
    bool detenido;
};

#endif // HELICOPTERO_H
