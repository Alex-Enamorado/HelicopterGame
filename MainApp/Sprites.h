#ifndef SPRITES_H
#define SPRITES_H

#include <QPixmap>
#include <QString>

/*
 * Sprites
 * ---------------------------------------------------------------------------
 * Funciones de apoyo para cargar las imágenes del juego.
 *
 * Las imágenes de la carpeta Recursos tienen un "halo" casi invisible
 * alrededor del dibujo (píxeles con transparencia de 1 a 20). Si no se
 * elimina, la colisión por silueta (MaskShape) considera ese halo como
 * parte sólida y los choques se detectan antes de que el dibujo se toque.
 */
namespace Sprites {

// Carga una imagen y vuelve totalmente transparentes los píxeles cuya
// opacidad sea menor que 'umbralAlfa' (0..255). Si la imagen no existe,
// devuelve un QPixmap nulo (isNull() == true).
QPixmap cargar(const QString &ruta, int umbralAlfa = 40);

} // namespace Sprites

#endif // SPRITES_H
