#include "helicoptero.h"
#include <QPixmap>
#include <algorithm>

using namespace std;
Helicoptero::Helicoptero() {
    QPixmap img(":/img/helicoptero.png");
    setPixmap(img.scaledToWidth(120, Qt::SmoothTransformation));
}


void Helicoptero::actualizar(double dt){
    velocidadY += gravedad * dt;
    velocidadY = clamp(velocidadY,-450.0,450.0);
    setY(y() + velocidadY * dt);
}

void Helicoptero::aterrizar(double ySuelo)
{
    setY(ySuelo - pixmap().height());
    velocidadY = 0;
}
