#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>

class QPushButton;

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
