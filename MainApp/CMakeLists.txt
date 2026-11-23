cmake_minimum_required(VERSION 3.16)

project(HelicopterRescue VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Herramientas automáticas de Qt:
#   AUTOMOC -> procesa las clases con Q_OBJECT (señales y slots)
#   AUTORCC -> compila el archivo .qrc (imágenes embebidas)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Se usa Qt 6 si está instalado; si no, Qt 5 (5.15 recomendado).
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(ARCHIVOS_FUENTE
    MainApp.cpp
    Configuracion.h
    VentanaPrincipal.h      VentanaPrincipal.cpp
    MenuPrincipal.h         MenuPrincipal.cpp
    JuegoView.h             JuegoView.cpp
    JuegoScene.h            JuegoScene.cpp
    Helicoptero.h           Helicoptero.cpp
    ControladorJuego.h      ControladorJuego.cpp
    Sprites.h               Sprites.cpp
    recursos.qrc
)

add_executable(HelicopterRescue ${ARCHIVOS_FUENTE})

target_link_libraries(HelicopterRescue PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Los archivos fuente están en UTF-8 (tildes y ñ en los textos).
if(MSVC)
    target_compile_options(HelicopterRescue PRIVATE /utf-8)
endif()

# En Windows/macOS se genera una aplicación gráfica (sin consola extra).
set_target_properties(HelicopterRescue PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS HelicopterRescue
    BUNDLE  DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
