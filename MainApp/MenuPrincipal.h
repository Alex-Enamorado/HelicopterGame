#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include <QWidget>
#include <QPixmap>

class MenuPrincipal : public QWidget{
    Q_OBJECT

    public:
        explicit MenuPrincipal(QWidget *widgetPadre = nullptr);

    signals:
        void jugar();

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        QPixmap imagenFondo;
};

#endif
