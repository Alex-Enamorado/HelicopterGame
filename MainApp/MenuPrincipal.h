#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>

class QPushButton;

// Pantalla del menú: título y tres botones. Solo avisa qué botón se presionó.
class MenuPrincipal : public QWidget {
    Q_OBJECT

public:
    MenuPrincipal(QWidget* padre = nullptr);

signals:
    void iniciarPresionado();
    void instruccionesPresionado();
    void salirPresionado();

private:
    QPushButton* crearBoton(QString rutaImagen);
};

#endif
