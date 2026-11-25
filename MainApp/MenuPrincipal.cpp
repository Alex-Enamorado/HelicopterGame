#include "MenuPrincipal.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>
#include <QIcon>

namespace{
    // Crea un boton con la imagen del recurso
    QPushButton *crearBoton(const QString &ruta, QWidget *padre){
        QPixmap img(ruta);
        img = img.scaledToWidth(320, Qt::SmoothTransformation);

        auto *boton = new QPushButton(padre);
        boton->setIcon(QIcon(img));
        boton->setIconSize(img.size());
        boton->setFixedSize(img.size());
        boton->setFlat(true);
        boton->setFocusPolicy(Qt::NoFocus);
        boton->setStyleSheet("QPushButton{border:none;background:transparent;}");
        boton->setCursor(Qt::PointingHandCursor);
        return boton;
    }
}

MenuPrincipal::MenuPrincipal(QWidget *parent)
    : QWidget(parent), fondo(":/img/fondo.png"){

    auto *titulo = new QLabel("HELICOPTER\nRESCUE", this);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(
        "color:#ffd84a;"
        "font-size:64px;"
        "font-weight:bold;"
        "background:transparent;"
    );

    QPushButton *botonJugar = crearBoton(":/img/boton_jugar.png", this);
    QPushButton *botonInstrucciones = crearBoton(":/img/boton_instrucciones.png", this);
    QPushButton *botonSalir = crearBoton(":/img/boton_salir.png", this);

    connect(botonJugar, &QPushButton::clicked, this, &MenuPrincipal::jugar);
    // botonInstrucciones y botonSalir: todavia sin implementar

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(14);
    layout->addStretch(1);
    layout->addWidget(titulo, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(botonJugar, 0, Qt::AlignHCenter);
    layout->addWidget(botonInstrucciones, 0, Qt::AlignHCenter);
    layout->addWidget(botonSalir, 0, Qt::AlignHCenter);
    layout->addStretch(1);
}

void MenuPrincipal::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawPixmap(rect(), fondo);
}
