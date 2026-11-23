#ifndef CONFIGURACION_H
#define CONFIGURACION_H

/*
 * Configuracion.h
 * ---------------------------------------------------------------------------
 * Constantes globales del juego agrupadas en un solo lugar.
 * Cambiar un valor aquí afecta a todo el proyecto, sin buscar "números
 * mágicos" repartidos por el código.
 *
 * Unidades:
 *   - Distancias en píxeles de escena (px).
 *   - Tiempo en segundos (s).
 *   - Velocidad en px/s y aceleración en px/s^2.
 */

namespace Config {

// ----- Escena ---------------------------------------------------------------
constexpr int ANCHO_ESCENA = 1024;   // Ancho lógico del escenario
constexpr int ALTO_ESCENA  = 600;    // Alto lógico del escenario

// ----- Actualización del juego ---------------------------------------------
constexpr int    INTERVALO_TIMER_MS = 16;    // ~60 actualizaciones por segundo
constexpr double DELTA_TIME_MAXIMO  = 0.05;  // Evita saltos enormes si la ventana se congela

// ----- Helicóptero ----------------------------------------------------------
constexpr double HELI_ANCHO           = 110.0;  // Ancho del sprite en escena
constexpr double HELI_POS_X_INICIAL   = 220.0;  // Posición horizontal fija (por ahora)
constexpr double HELI_POS_Y_INICIAL   = 300.0;  // Posición vertical inicial (centro)

constexpr double GRAVEDAD             = 900.0;   // Aceleración hacia abajo
constexpr double EMPUJE_MOTOR         = 1800.0;  // Aceleración hacia arriba al presionar ESPACIO
constexpr double VELOCIDAD_MAX_SUBIDA = 420.0;   // Límite de velocidad ascendente
constexpr double VELOCIDAD_MAX_CAIDA  = 520.0;   // Límite de velocidad descendente

constexpr double ANGULO_MAXIMO        = 18.0;    // Inclinación máxima en grados
constexpr double SUAVIZADO_ROTACION   = 8.0;     // Qué tan rápido alcanza el ángulo objetivo

// ----- Escenario ------------------------------------------------------------
constexpr double ALTO_TECHO = 120.0;   // Alto visual de la franja del techo
constexpr double ALTO_SUELO = 70.0;    // Alto visual de la franja del suelo

} // namespace Config

#endif // CONFIGURACION_H
