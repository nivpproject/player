#include "graphicitems.h"


GraphicsTextItem::GraphicsTextItem()
{

}

void GraphicsTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsTextItem::hoverEnterEvent(event);
    emit hoverEnter();
}

void GraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsTextItem::hoverLeaveEvent(event);
    emit hoverLeave();
}

void GraphicsTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsTextItem::hoverMoveEvent(event);
    emit hoverMove(event->pos().x(), event->pos().y());
}

void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseDoubleClickEvent(event);
    emit doubleClick(event->pos().x(), event->pos().y());
}

void GraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
    emit mouseMove(event->pos().x(), event->pos().y());
}

void GraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
    emit mousePress(event->pos().x(), event->pos().y());
}

void GraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);
    emit mouseRelease(event->pos().x(), event->pos().y());
}


void GraphicsPixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
        qDebug() << event;
    QGraphicsPixmapItem::hoverEnterEvent(event);
    emit hoverEnter();
}

void GraphicsPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
        qDebug() << event;
    QGraphicsPixmapItem::hoverLeaveEvent(event);
    emit hoverLeave();
}

void GraphicsPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPixmapItem::hoverMoveEvent(event);
    emit hoverMove(event->pos().x(), event->pos().y());
}

void GraphicsPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
    emit doubleClick(event->pos().x(), event->pos().y());
}

void GraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
    emit mouseMove(event->pos().x(), event->pos().y());
}

void GraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    emit mousePress(event->pos().x(), event->pos().y());
}

void GraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    emit mouseRelease(event->pos().x(), event->pos().y());
}

void GraphicsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverEnterEvent(event);
    emit hoverEnter();
}

void GraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsRectItem::hoverLeaveEvent(event);
    emit hoverLeave();
}

void GraphicsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverMoveEvent(event);
    emit hoverMove(event->pos().x(), event->pos().y());
}

void GraphicsRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseDoubleClickEvent(event);
    emit doubleClick(event->pos().x(), event->pos().y());
}

void GraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseMoveEvent(event);
    emit mouseMove(event->pos().x(), event->pos().y());
}

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mousePressEvent(event);
    emit mousePress(event->pos().x(), event->pos().y());
}

void GraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);
    emit mouseRelease(event->pos().x(), event->pos().y());
}

