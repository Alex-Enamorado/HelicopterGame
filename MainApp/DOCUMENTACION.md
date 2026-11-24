# HELICOPTER RESCUE — Avance 1

## Cómo abrirlo en Qt Creator

1. *File → Open File or Project…* → elegir `MainApp/CMakeLists.txt`.
2. Elegir un kit de escritorio (por ejemplo *Desktop Qt 6.x MinGW 64-bit*) → *Configure Project*.
3. **Ctrl+R** para compilar y ejecutar.

Funciona con Qt 6 o Qt 5.15 y CMake. Las imágenes se toman de `../Recursos`
por medio de `recursos.qrc` y quedan dentro del ejecutable.

## Archivos

| Archivo | Qué hace |
|---|---|
| `MainApp.cpp` | `main()`: crea la aplicación y la ventana |
| `VentanaPrincipal` | Ventana de 1024×600 que cambia entre el menú y el juego (`QStackedWidget`) |
| `MenuPrincipal` | Título y botones Iniciar Misión, Instrucciones y Salir. Emite una señal por botón |
| `JuegoView` | Muestra la escena y convierte las teclas en señales |
| `JuegoScene` | Arma el escenario (fondo, techo, suelo, helicóptero, mensaje) y revisa colisiones |
| `Helicoptero` | Posición, velocidad, aceleración, gravedad, movimiento lateral y rotación |
| `ControladorJuego` | Tiene el `QTimer`, calcula el delta time y decide cuándo empieza y termina la partida |

## Física

En Qt el eje Y crece hacia abajo, así que la gravedad es positiva y el empuje se resta:

```
aceleracion = gravedad                (900)        sin ESPACIO
aceleracion = gravedad - empuje       (900 - 1800) con ESPACIO

vy = vy + aceleracion * deltaTime
y  = y  + vy * deltaTime
```

* **deltaTime** son los segundos reales desde el tick anterior (`QElapsedTimer`).
  Así el helicóptero se mueve igual en cualquier computadora, aunque el
  `QTimer` no sea exacto.
* La velocidad se limita a ±450 px/s.
* **Flecha abajo:** suma otra `gravedad` a la aceleración, así que baja más rápido.
* **Movimiento lateral:** con IZQUIERDA o DERECHA la `velocidadX` cambia
  `1500 * deltaTime` por cuadro (máximo ±350 px/s). Al soltar la tecla, la
  `friccion` (1200) la frena hasta 0. `x = x + velocidadX * deltaTime`.
  La escena mantiene al helicóptero dentro de la pantalla y, al tocar un lado, su `velocidadX` vuelve a 0.
* **Inclinación:** `anguloObjetivo = vy / 25`, como máximo 18°. En cada cuadro
  la rotación avanza solo una parte hacia ese ángulo, para que no gire de golpe:
  `rotacion += (anguloObjetivo - rotacion) * 5 * deltaTime`.

## Teclado

`JuegoView` sobrescribe `keyPressEvent` y `keyReleaseEvent`:

| Tecla | Señal | Efecto |
|---|---|---|
| ESPACIO o ↑ (presionar) | `teclaImpulso(true)` | Arranca el vuelo y activa el impulso |
| ESPACIO o ↑ (soltar) | `teclaImpulso(false)` | Quita el impulso (si no sigue presionada la otra tecla); cae por gravedad |
| ↓ | `teclaAbajo(bool)` | Baja más rápido mientras se mantiene |
| ← y → | `teclaIzquierda(bool)`, `teclaDerecha(bool)` | Mueve el helicóptero a los lados mientras se mantienen |
| ENTER | `teclaEnter()` | Reintenta después de chocar |
| ESC | `teclaEscape()` | Vuelve al menú |

`isAutoRepeat()` ignora las repeticiones automáticas de una tecla mantenida.

## Colisiones

* **Techo:** el helicóptero choca si su borde superior pasa `LIMITE_TECHO` (95).
  Se detiene el timer y aparece "MISIÓN TERMINADA".
* **Suelo:** no se pierde. Si el borde inferior pasa `LIMITE_SUELO` (540),
  `JuegoScene` llama a `Helicoptero::aterrizar()`, que lo deja apoyado en el
  suelo con `velocidadY = 0`. Puede quedarse ahí o despegar con ESPACIO / ↑.
* **Lados:** `Helicoptero::limitarX()` evita que salga de la pantalla.

## Memoria y punteros

* Los widgets y el `QTimer` tienen un **padre** en Qt. Qt los borra al borrar
  al padre, así que no se les hace `delete`.
* Los objetos agregados a la escena con `addItem`/`addPixmap` (helicóptero,
  mensaje, techo…) los **borra la escena**.
* La `JuegoScene` se crea con `new` sin padre en `ControladorJuego`, y su
  destructor hace `delete escena; escena = nullptr;`.
* Todos los punteros miembro reciben un valor en el constructor, antes de usarse,
  y después del `delete` la escena se deja en `nullptr`.
* No se usa `std::vector`, `std::list`, `QList`, `std::queue` ni `std::stack`.

## Pruebas

| Prueba | Resultado esperado |
|---|---|
| Ejecutar | Aparece el menú con título y 3 botones |
| Instrucciones | Cuadro con los controles |
| Salir | La aplicación se cierra |
| Iniciar Misión | Cueva con "Presiona ESPACIO para despegar" |
| Mantener ESPACIO | Sube y se inclina con la nariz hacia arriba |
| Soltar ESPACIO | Baja y se inclina con la nariz hacia abajo, suavemente |
| No presionar nada | Cae y se queda apoyado en el suelo, sin perder |
| ← / → | Se mueve a los lados y frena al soltar; no sale de la pantalla |
| ↑ | Igual que ESPACIO |
| ↓ | Baja más rápido |
| En el suelo, ESPACIO / ↑ | Vuelve a despegar |
| Mantener ESPACIO siempre | Choca con el techo → "MISIÓN TERMINADA" |
| ENTER tras chocar | Reinicia la partida |
| ESC | Vuelve al menú |
