cmake_minimum_required(VERSION 3.16)

project(HelicopterRescue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)   # Señales y slots (Q_OBJECT)
set(CMAKE_AUTORCC ON)   # Imágenes del archivo .qrc

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(HelicopterRescue WIN32
    MainApp.cpp
    VentanaPrincipal.h  VentanaPrincipal.cpp
    MenuPrincipal.h     MenuPrincipal.cpp
    JuegoView.h         JuegoView.cpp
    JuegoScene.h        JuegoScene.cpp
    Helicoptero.h       Helicoptero.cpp
    ControladorJuego.h  ControladorJuego.cpp
    recursos.qrc
)

target_link_libraries(HelicopterRescue PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(MSVC)
    target_compile_options(HelicopterRescue PRIVATE /utf-8)
endif()
