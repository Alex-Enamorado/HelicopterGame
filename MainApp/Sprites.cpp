#include "Sprites.h"

#include <QImage>
#include <QRgb>

namespace Sprites {

QPixmap cargar(const QString &ruta, int umbralAlfa)
{
    QImage imagen(ruta);
    if (imagen.isNull())
        return QPixmap();

    // Formato con canal alfa de 32 bits por píxel (un QRgb por píxel)
    imagen = imagen.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < imagen.height(); ++y) {
        // scanLine devuelve un puntero al inicio de la fila 'y'.
        // Se recorre la fila avanzando el puntero píxel por píxel.
        QRgb *pixel = reinterpret_cast<QRgb *>(imagen.scanLine(y));
        QRgb *finFila = pixel + imagen.width();
        for (; pixel != finFila; ++pixel) {
            if (qAlpha(*pixel) < umbralAlfa)
                *pixel = qRgba(0, 0, 0, 0);
        }
    }
    return QPixmap::fromImage(imagen);
}

} // namespace Sprites
