#ifndef JUEGOSCENE_H
#define JUEGOSCENE_H

#include <QGraphicsScene>
#include <QString>

class Helicoptero;
class QGraphicsPixmapItem;
class QGraphicsRectItem;
class QGraphicsSimpleTextItem;
class QGraphicsTextItem;
class QPixmap;

/*
 * JuegoScene
 * ---------------------------------------------------------------------------
 * Administra todo lo que existe dentro del escenario 2D:
 *   - Fondo de cueva, techo y suelo.
 *   - El helicóptero.
 *   - El HUD (datos de vuelo) y la capa de mensajes.
 *
 * Expone operaciones de alto nivel (reiniciar, actualizar, verificar
 * colisiones, mostrar mensaje) para que el ControladorJuego coordine la
 * partida sin conocer los detalles gráficos.
 *
 * Propiedad de memoria: todos los items creados con new y agregados con
 * addItem() pasan a ser propiedad de la escena. Al destruirse la escena,
 * Qt los libera automáticamente. Los punteros guardados aquí son solo
 * "accesos directos": NUNCA se les hace delete manual.
 */
class JuegoScene : public QGraphicsScene
{
    Q_OBJECT

public:
    // Resultado de la detección de colisiones
    enum class Impacto { Ninguno, Techo, Suelo };

    explicit JuegoScene(QObject *padre = nullptr);
    ~JuegoScene() override;

    void reiniciar();                       // Deja la escena lista para una nueva partida
    void actualizar(double deltaTime);      // Física del helicóptero + HUD
    Impacto verificarColisiones() const;    // ¿Tocó el techo o el suelo?

    void mostrarMensaje(const QString &titulo, const QString &detalle,
                        bool oscurecerFondo);
    void ocultarMensaje();

    Helicoptero *getHelicoptero() const { return helicoptero; }
    double getTiempoVuelo() const { return tiempoVuelo; }

private:
    void crearFondo();
    void crearTecho();
    void crearSuelo();
    void crearHelicoptero();
    void crearHUD();
    void crearCapaMensaje();
    void actualizarHUD();

    // Une dos secciones de imagen, alternándolas, hasta cubrir el ancho.
    static QPixmap construirFranja(const QString &rutaA, const QString &rutaB,
                                   int ancho, int alto,
                                   const QColor &colorRespaldo);

    // Punteros NO propietarios (la escena es la dueña). Se inicializan en
    // nullptr para que nunca contengan basura.
    QGraphicsPixmapItem     *fondo       = nullptr;
    QGraphicsPixmapItem     *techo       = nullptr;
    QGraphicsPixmapItem     *suelo       = nullptr;
    Helicoptero             *helicoptero = nullptr;
    QGraphicsRectItem       *panelHUD    = nullptr;
    QGraphicsSimpleTextItem *textoHUD    = nullptr;
    QGraphicsRectItem       *capaMensaje = nullptr;
    QGraphicsTextItem       *textoMensaje = nullptr;

    double tiempoVuelo = 0.0;   // Segundos de vuelo de la partida actual
};

#endif // JUEGOSCENE_H
