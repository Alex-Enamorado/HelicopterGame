#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include <QWidget>
#include <QPixmap>

class MenuPrincipal : public QWidget{
    Q_OBJECT

    public:
        explicit MenuPrincipal(QWidget *parent = nullptr);

    signals:
        void jugar();

    protected:
        void paintEvent(QPaintEvent *e) override;

    private:
        QPixmap fondo;
};

#endif // MENUPRINCIPAL_H
