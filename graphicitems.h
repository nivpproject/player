#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>


class GraphicsTextItem : public QGraphicsTextItem {
    Q_OBJECT
public:
    explicit GraphicsTextItem();

    virtual void 	hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void 	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void hoverEnter();
    void hoverLeave();
    void hoverMove(int x, int y);

    void doubleClick(int x, int y);
    void mouseMove(int x, int y);
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);
};


class GraphicsPixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GraphicsPixmapItem() {
        this->setAcceptHoverEvents(true);
    }

    virtual void 	hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void 	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void hoverEnter();
    void hoverLeave();
    void hoverMove(int x, int y);

    void doubleClick(int x, int y);
    void mouseMove(int x, int y);
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);
};

class GraphicsRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit GraphicsRectItem() {
        this->setAcceptHoverEvents(true);
    }

    virtual void 	hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void 	hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void 	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void 	mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void hoverEnter();
    void hoverLeave();
    void hoverMove(int x, int y);

    void doubleClick(int x, int y);
    void mouseMove(int x, int y);
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);
};
