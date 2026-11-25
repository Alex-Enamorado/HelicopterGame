#include "MenuPrincipal.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QIcon>

namespace{
    QPushButton *crearBotonConImagen(const QString &rutaImagen, QWidget *widgetPadre){
        QPixmap imagenBoton(rutaImagen);
        imagenBoton = imagenBoton.scaledToWidth(320, Qt::SmoothTransformation);

        auto *nuevoBoton = new QPushButton(widgetPadre);
        nuevoBoton->setIcon(QIcon(imagenBoton));
        nuevoBoton->setIconSize(imagenBoton.size());
        nuevoBoton->setFixedSize(imagenBoton.size());
        nuevoBoton->setFlat(true);
        nuevoBoton->setFocusPolicy(Qt::NoFocus);
        nuevoBoton->setStyleSheet("QPushButton{border:none;background:transparent;}");
        nuevoBoton->setCursor(Qt::PointingHandCursor);
        return nuevoBoton;
    }
}

MenuPrincipal::MenuPrincipal(QWidget *widgetPadre)
    : QWidget(widgetPadre), imagenFondo(":/img/fondo.png"){

    auto *tituloDelJuego = new QLabel("HELICOPTER\nRESCUE", this);
    tituloDelJuego->setAlignment(Qt::AlignCenter);
    tituloDelJuego->setStyleSheet(
        "color:#ffd84a;"
        "font-size:64px;"
        "font-weight:bold;"
        "background:transparent;"
    );

    QPushButton *botonJugar = crearBotonConImagen(":/img/boton_jugar.png", this);
    QPushButton *botonInstrucciones = crearBotonConImagen(":/img/boton_instrucciones.png", this);
    QPushButton *botonSalir = crearBotonConImagen(":/img/boton_salir.png", this);

    connect(botonJugar, &QPushButton::clicked, this, &MenuPrincipal::jugar);

    auto *disposicionVertical = new QVBoxLayout(this);
    disposicionVertical->setSpacing(14);
    disposicionVertical->addStretch(1);
    disposicionVertical->addWidget(tituloDelJuego, 0, Qt::AlignHCenter);
    disposicionVertical->addSpacing(20);
    disposicionVertical->addWidget(botonJugar, 0, Qt::AlignHCenter);
    disposicionVertical->addWidget(botonInstrucciones, 0, Qt::AlignHCenter);
    disposicionVertical->addWidget(botonSalir, 0, Qt::AlignHCenter);
    disposicionVertical->addStretch(1);
}

void MenuPrincipal::paintEvent(QPaintEvent *){
    QPainter pintor(this);
    pintor.drawPixmap(rect(), imagenFondo);
}
