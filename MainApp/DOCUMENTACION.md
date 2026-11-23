# HELICOPTER RESCUE — Avance 1

Base del videojuego: menú principal, escenario de cueva, helicóptero con física
basada en delta time, colisiones con techo y suelo, y ciclo de juego con `QTimer`.

---

## 1. Decisiones técnicas (versión de Qt y compilación)

| Tema | Decisión | Motivo |
|---|---|---|
| Versión de Qt | **Qt 6** (probado con 6.8.3). También compila con **Qt 5.15** | `CMakeLists.txt` busca primero Qt6 y, si no lo encuentra, Qt5 (`find_package(QT NAMES Qt6 Qt5 ...)`). No se usa ninguna API exclusiva de Qt 6. |
| Sistema de compilación | **CMake ≥ 3.16** | Es el sistema por defecto de Qt Creator para proyectos nuevos. |
| Compilador | MinGW 64-bit (probado con MinGW 13.1) o MSVC | Con MSVC se agrega `/utf-8` automáticamente para que las tildes se vean bien. |
| Estándar C++ | C++17 | Lo exige Qt 6. |
| Recursos | Archivo `recursos.qrc` con `AUTORCC` | Las imágenes quedan **dentro del ejecutable**: no hay que copiar carpetas junto al `.exe`. |

El proyecto se compiló con Qt 6.8.3 + MinGW 13.1 sin errores ni advertencias
(`-Wall -Wextra`).

---

## 2. Estructura del proyecto

```
HelicopterGame/
├── Recursos/                    ← imágenes originales (se usan desde el .qrc)
└── MainApp/                     ← proyecto de Qt Creator
    ├── CMakeLists.txt
    ├── recursos.qrc
    ├── MainApp.cpp              ← punto de entrada (main)
    ├── Configuracion.h          ← constantes del juego (física, tamaños)
    ├── VentanaPrincipal.h/.cpp
    ├── MenuPrincipal.h/.cpp
    ├── JuegoView.h/.cpp
    ├── JuegoScene.h/.cpp
    ├── Helicoptero.h/.cpp
    ├── ControladorJuego.h/.cpp
    └── Sprites.h/.cpp           ← carga de imágenes con limpieza de bordes
```

Se agregaron dos archivos que no estaban en la estructura pedida:

* **`Configuracion.h`**: reúne en un solo lugar la gravedad, el empuje, las
  velocidades máximas y los tamaños, para ajustar el juego sin buscar números
  repartidos por el código.
* **`Sprites.h/.cpp`**: las imágenes de `Recursos/` tienen un halo casi
  invisible alrededor del dibujo. Sin limpiarlo, la colisión detectaba el
  choque unos 20 px antes de que el helicóptero tocara el suelo.

---

## 3. Abrir y ejecutar en Qt Creator

1. Abrir **Qt Creator** → *File* → *Open File or Project…*
2. Seleccionar **`MainApp/CMakeLists.txt`**.
3. En *Configure Project*, marcar un kit de escritorio (por ejemplo
   *Desktop Qt 6.x MinGW 64-bit*) → **Configure Project**.
4. Presionar **Ctrl+R** (o el botón verde ▶) para compilar y ejecutar.

Si aparece *"Could not find a package configuration file provided by Qt6"*,
el kit no tiene Qt configurado: revisar en *Edit → Preferences → Kits* que el
kit elegido tenga una versión de Qt asignada.

**Agregar nuevas imágenes:** copiar el PNG en `Recursos/`, agregar una línea
en `recursos.qrc`
(`<file alias="nombre.png">../Recursos/archivo.png</file>`) y usarla en el
código como `":/img/nombre.png"`. Si una imagen no se encuentra, el juego
dibuja una figura temporal en su lugar y no se cierra.

---

## 4. Explicación de cada clase

### `MainApp.cpp`
Solo crea `QApplication` y la `VentanaPrincipal`, y arranca el ciclo de
eventos con `app.exec()`. No tiene lógica del juego.

### `VentanaPrincipal` (hereda de `QMainWindow`)
Contiene un `QStackedWidget` con dos pantallas: índice 0 el **menú** e índice
1 la **vista del juego**. Conecta las señales del menú:
* *Iniciar Misión* → cambia a la vista del juego y llama a
  `ControladorJuego::iniciarPartida()`.
* *Instrucciones* → muestra un `QMessageBox` con los controles.
* *Salir* → `close()`; al cerrarse la última ventana, la aplicación termina.

### `MenuPrincipal` (hereda de `QWidget`)
Es solo interfaz: título **HELICOPTER RESCUE**, subtítulo, helicóptero
decorativo y los tres botones gráficos dentro de un panel verde militar. El
fondo de montañas se dibuja en `paintEvent` con un velo oscuro y una viñeta.
Cuando se presiona un botón, el menú **emite una señal**; no sabe qué ocurre
después.

### `JuegoView` (hereda de `QGraphicsView`)
* Muestra la escena escalada a la ventana sin deformarla (`fitInView` con
  `KeepAspectRatio`).
* Recibe el teclado y lo convierte en señales del juego: `impulsoCambiado(bool)`,
  `reintentarSolicitado()` y `menuSolicitado()`.

### `JuegoScene` (hereda de `QGraphicsScene`)
Construye y administra el escenario de 1024×600:
* **Fondo** de cueva (zValue 0).
* **Techo** y **suelo** (zValue 5): cada uno es un único
  `QGraphicsPixmapItem` formado al alternar las dos secciones de imagen. Usan
  `MaskShape`, así que la colisión respeta la forma de las estalactitas y del
  suelo.
* **Helicóptero** (zValue 10).
* **HUD** con tiempo de vuelo, altura, velocidad, aceleración y estado del
  motor (zValue 20).
* **Capa de mensajes** para "¿Listo para despegar?" y "Misión terminada"
  (zValue 50).

Métodos principales: `reiniciar()`, `actualizar(dt)`, `verificarColisiones()`,
`mostrarMensaje()` y `ocultarMensaje()`.

### `Helicoptero` (hereda de `QGraphicsPixmapItem`)
Guarda posición (`posX`, `posY`), `velocidadY`, `aceleracion`, `gravedad`,
`empujeMotor` y `rotacionActual`. Calcula la física en `actualizar(dt)`. No
conoce el teclado: solo recibe `setImpulso(true/false)`. Su origen está en el
centro de la imagen (`setOffset`), por lo que gira alrededor de su centro.

### `ControladorJuego` (hereda de `QObject`)
Coordina la partida:
* Crea la `JuegoScene` y el `QTimer`.
* Lleva el estado: `Inactivo → Preparado → EnCurso → Terminada`.
* En cada *tick* del timer calcula el **delta time**, actualiza la escena y
  revisa colisiones.
* Si hay impacto, detiene el timer y el helicóptero y muestra el mensaje.

### Flujo de una partida

```
[Menú] --Iniciar Misión--> [Preparado: mensaje "¿Listo?", física detenida]
                                 | ESPACIO
                                 v
                           [EnCurso: QTimer cada 16 ms]
                                 | impacto techo/suelo
                                 v
                           [Terminada: "MISIÓN TERMINADA"]
                              | ENTER/R          | ESC (en cualquier momento)
                              v                  v
                         [Preparado]          [Menú]
```

---

## 5. Ecuaciones físicas

En Qt el eje **Y crece hacia abajo**. Por eso la gravedad es positiva y el
empuje del motor se resta.

```
aceleracion = gravedad - (impulsoActivo ? empujeMotor : 0)

vy = vy + aceleracion * deltaTime        (velocidad)
y  = y  + vy * deltaTime                 (posición)
```

| Constante (`Configuracion.h`) | Valor | Efecto |
|---|---|---|
| `GRAVEDAD` | 900 px/s² | Caída cuando no se presiona nada |
| `EMPUJE_MOTOR` | 1800 px/s² | Con ESPACIO la aceleración neta es −900 px/s² (hacia arriba) |
| `VELOCIDAD_MAX_SUBIDA` | 420 px/s | Límite de velocidad al subir |
| `VELOCIDAD_MAX_CAIDA` | 520 px/s | Límite de velocidad al caer |

**¿Por qué delta time?** El `QTimer` pide un tick cada 16 ms, pero el
sistema operativo no lo garantiza (puede tardar 15, 17 o 30 ms). Con un
`QElapsedTimer` se mide el tiempo **real** transcurrido y todas las
ecuaciones se multiplican por él. Así, en una computadora lenta hay menos
cuadros, pero cada uno avanza más, y el helicóptero recorre la misma
distancia por segundo. `deltaTime` se limita a 0.05 s para que una pausa
larga (por ejemplo, al arrastrar la ventana) no haga atravesar el suelo.

**Inclinación suave:**

```
anguloObjetivo = (vy / velocidadMaxima) * 18°      (negativo al subir: nariz arriba)
rotacion = rotacion + (anguloObjetivo - rotacion) * min(1, 8 * deltaTime)
```

En lugar de saltar al ángulo objetivo, en cada cuadro solo se recorre una
fracción de la diferencia. Como esa fracción depende de `deltaTime`, la
suavidad es igual a cualquier FPS.

---

## 6. Eventos de teclado

`JuegoView` sobrescribe:

| Evento | Tecla | Acción |
|---|---|---|
| `keyPressEvent` | ESPACIO | `emit impulsoCambiado(true)` |
| `keyReleaseEvent` | ESPACIO | `emit impulsoCambiado(false)` |
| `keyPressEvent` | ENTER / R | `emit reintentarSolicitado()` |
| `keyPressEvent` | ESC | `emit menuSolicitado()` |
| `focusOutEvent` | — | `emit impulsoCambiado(false)` |

Detalles importantes:
* **`isAutoRepeat()`**: al mantener una tecla, el sistema operativo envía
  muchos *press/release* repetidos. Se ignoran y solo cuentan el primer
  *press* y el *release* real.
* **`focusOutEvent`**: si la ventana pierde el foco con ESPACIO presionado,
  nunca llega el *release*. Por eso se suelta el impulso por seguridad.
* `setFocusPolicy(Qt::StrongFocus)` permite que la vista reciba el teclado.

El flujo es *Vista (teclado) → señal → ControladorJuego → Helicoptero::setImpulso*.
La física nunca lee el teclado directamente.

---

## 7. Punteros y administración de memoria

### Árbol de propiedad

```
VentanaPrincipal                  (en la pila, dentro de main)
 ├── QStackedWidget               hijo → Qt lo libera
 │    ├── MenuPrincipal           hijo → Qt lo libera
 │    │    └── botones, labels…   hijos → Qt los libera
 │    └── JuegoView               hijo → Qt lo libera
 └── ControladorJuego             hijo → Qt lo libera
      ├── QTimer                  hijo → Qt lo libera
      └── JuegoScene              SIN padre → delete manual en ~ControladorJuego
           └── items (fondo, techo, suelo, Helicoptero, HUD…)
                                  addItem() → la escena los libera
```

### Reglas aplicadas

1. **Objetos con padre Qt** (`new QTimer(this)`, widgets en layouts,
   `addWidget`): Qt los libera al destruir el padre. **No se les hace
   `delete`**, para no liberarlos dos veces.
2. **Items de la escena**: después de `addItem()`, la escena es la dueña. Los
   punteros en `JuegoScene` (`helicoptero`, `techo`, …) son solo accesos
   directos; en el destructor se ponen en `nullptr` y no se borran.
3. **`JuegoScene` se libera de forma explícita**: se crea con `new` sin padre
   y `~ControladorJuego()` hace `delete escena; escena = nullptr;`. Es el
   único `delete` manual del proyecto, porque es el único objeto sin otro
   dueño.
4. **Prevención de punteros colgantes:**
   * Antes de borrar la escena se ejecuta `vista->setScene(nullptr)`, así la
     vista nunca apunta a una escena destruida.
   * La vista se guarda como `QPointer<JuegoView>`, que se vuelve `nullptr`
     automáticamente si la vista se destruye primero. Esto evita un
     *segmentation fault* según el orden de destrucción.
5. **Inicialización segura:** todos los punteros miembro se inicializan en
   `nullptr` en el `.h`, y los métodos verifican `if (puntero)` antes de usarlos.
6. **Objetos en la pila:** `VentanaPrincipal` en `main` y el `QMessageBox` de
   instrucciones se liberan solos al salir de su función.
7. **Uso de punteros en `Sprites::cargar`:** se recorre cada fila de la
   imagen con un puntero `QRgb*` (`scanLine`) que avanza píxel por píxel.

### Preparado para los siguientes avances
No se usa `std::vector`, `std::list`, `QList`, `std::queue` ni `std::stack`.
La escena guarda cada elemento con un puntero propio. Cuando se agreguen
misiles, enemigos o civiles, `JuegoScene` podrá tener una lista enlazada
propia (`Nodo* cabeza`). Cada nodo apuntará a un item de la escena y la
escena seguirá siendo la dueña del item gráfico. Lo mismo aplica para las
colas y pilas manuales.

---

## 8. Pruebas para verificar cada funcionalidad

### Pruebas manuales

| # | Prueba | Resultado esperado |
|---|---|---|
| 1 | Ejecutar el programa | Se abre el menú con título, helicóptero y los 3 botones sobre el fondo de montañas |
| 2 | Pasar el mouse sobre un botón | Borde dorado y cursor de mano |
| 3 | Clic en **Instrucciones** | Cuadro con los controles; "Entendido" lo cierra |
| 4 | Clic en **Salir** | La aplicación se cierra sin errores |
| 5 | Clic en **Iniciar Misión** | Escenario de cueva con mensaje "¿Listo para despegar?" y helicóptero quieto |
| 6 | Presionar y mantener ESPACIO | El helicóptero sube, se inclina con la nariz hacia arriba y el HUD muestra `MOTOR: IMPULSO` y velocidad negativa |
| 7 | Soltar ESPACIO | Desacelera, empieza a caer y se inclina suavemente con la nariz hacia abajo |
| 8 | Toques cortos de ESPACIO | Se puede mantener una altura estable sin giros bruscos |
| 9 | No presionar nada | Cae hasta el suelo → "MISIÓN TERMINADA… contra el suelo" y se queda quieto |
| 10 | Mantener ESPACIO siempre | Choca con las estalactitas → "…contra el techo de la cueva" |
| 11 | ENTER o R tras el impacto | Vuelve a "¿Listo para despegar?" con el helicóptero en el centro |
| 12 | ESC durante el vuelo | Vuelve al menú y el juego se detiene |
| 13 | Redimensionar/maximizar la ventana | El escenario se escala sin deformarse |
| 14 | Mantener ESPACIO y cambiar de ventana (Alt+Tab) | Al volver, el motor no queda trabado en IMPULSO |

### Pruebas automáticas realizadas
Se ejecutó un programa de prueba aparte (no incluido en el proyecto) que
compila las clases y simula el teclado. Resultado: **23 de 23 pruebas OK**.

* Caída libre de 0.5 s: y = 416.3 px (teórico ½·g·t² ≈ 412.5 px; la
  diferencia se debe a la integración por pasos).
* **Delta time:** el mismo vuelo simulado a 60 FPS y a 20 FPS termina en
  y = 90.75 y y = 91.50 (diferencia < 1 px).
* Inclinación: nariz hacia arriba al subir y hacia abajo al bajar. Nunca
  supera 18° y el cambio máximo por cuadro fue de 0.61°.
* El límite de velocidad y `detener()` funcionan.
* Flujo completo: menú → preparado → ESPACIO inicia → sube → cae → impacto
  con el suelo → se detiene → R reinicia → impacto con el techo → ESC vuelve
  al menú.
* 20 ciclos menú↔juego, y crear/destruir la ventana 10 veces con una partida
  activa, sin fallos (comprueba la liberación de memoria y el orden de
  destrucción).
