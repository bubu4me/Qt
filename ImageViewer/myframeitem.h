#ifndef MYFRAMEITEM_H
#define MYFRAMEITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

class MyFrameItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyFrameItem(QGraphicsPixmapItem *parent = nullptr);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void reset();

signals:

public slots:
};

#endif // MYFRAMEITEM_H
