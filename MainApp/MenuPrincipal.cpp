#include "MenuPrincipal.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace std;

MenuPrincipal::MenuPrincipal(QWidget* padre) : QWidget(padre){
    QPixmap fondo(":/img/fondo_montanas.png");
    fondo = fondo.scaled(1024, 600);
    QPalette paleta = palette();
    paleta.setBrush(QPalette::Window, QBrush(fondo));
    setPalette(paleta);
    setAutoFillBackground(true);

    QLabel* titulo = new QLabel("HELICOPTER RESCUE");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("color: #e8d27a; font: bold 56px 'Impact';"
                          "background: rgba(0, 0, 0, 140); padding: 10px;");

    QPushButton* botonIniciar = crearBoton(":/img/boton_iniciar.png");
    QPushButton* botonInstrucciones = crearBoton(":/img/boton_instrucciones.png");
    QPushButton* botonSalir = crearBoton(":/img/boton_salir.png");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(titulo);
    layout->addSpacing(30);
    layout->addWidget(botonIniciar, 0, Qt::AlignHCenter);
    layout->addWidget(botonInstrucciones, 0, Qt::AlignHCenter);
    layout->addWidget(botonSalir, 0, Qt::AlignHCenter);
    layout->addStretch();

    connect(botonIniciar, &QPushButton::clicked, this, &MenuPrincipal::iniciarPresionado);
    connect(botonInstrucciones, &QPushButton::clicked, this, &MenuPrincipal::instruccionesPresionado);
    connect(botonSalir, &QPushButton::clicked, this, &MenuPrincipal::salirPresionado);
}

QPushButton* MenuPrincipal::crearBoton(QString rutaImagen){
    QPixmap imagen(rutaImagen);

    QPushButton* boton = new QPushButton();
    boton->setIcon(QIcon(imagen));
    boton->setIconSize(imagen.size());
    boton->setCursor(Qt::PointingHandCursor);
    boton->setStyleSheet("QPushButton { border: none; }"
                         "QPushButton:hover { background: rgba(255, 255, 255, 40); }");
    return boton;
}
