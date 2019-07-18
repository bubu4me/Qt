#include "myframeitem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

MyFrameItem::MyFrameItem(QGraphicsPixmapItem *parent) :
    QGraphicsPixmapItem(parent)
{

}
void MyFrameItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "press";
}
void MyFrameItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(pos() + mapToParent(event->pos()) - mapToParent(event->lastPos()));
    qDebug() << "move";
}
void MyFrameItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    qDebug() << "wheel";
    qDebug() << event->delta();
    double s = scale();
    double percent = 0.1;
    if (event->delta() > 0) {
        s *= (1 + percent);
    }
    if (event->delta() < 0) {
        s /= (1 + percent);
    }
    setTransformOriginPoint(boundingRect().width() / 2, boundingRect().height() / 2);
    setScale(s);
}

void MyFrameItem::reset()
{
    setPos(0, 0);
    setScale(scale());
}








