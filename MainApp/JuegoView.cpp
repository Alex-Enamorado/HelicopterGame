#include "JuegoView.h"
#include <QKeyEvent>

using namespace std;

JuegoView::JuegoView(QWidget* padre) : QGraphicsView(padre){
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setRenderHint(QPainter::SmoothPixmapTransform);
}

void JuegoView::keyPressEvent(QKeyEvent* evento){
    if(evento->isAutoRepeat()){
        return;
    }

    if(evento->key() == Qt::Key_Space){
        emit teclaEspacio(true);
    } else if(evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter){
        emit teclaEnter();
    } else if(evento->key() == Qt::Key_Escape){
        emit teclaEscape();
    }
}

void JuegoView::keyReleaseEvent(QKeyEvent* evento){
    if(evento->isAutoRepeat()){
        return;
    }

    if(evento->key() == Qt::Key_Space){
        emit teclaEspacio(false);
    }
}
