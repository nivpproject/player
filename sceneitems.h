#ifndef SCENEITEMS_H
#define SCENEITEMS_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsVideoItem>
#include <QGraphicsRectItem>
#include <QJSValue>
#include <QFont>
#include <QImage>
#include <QPushButton>
#include <QBuffer>
#include <QByteArray>
#include <QMediaPlayer>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>
#include "niv.h"


class Utils
{
public:

    struct ImageDesc {
        static ImageDesc fromJSValue(const QJSValue &v);
        QString filename;
        int width, height;
    };

    struct BrushDesc {
        static BrushDesc fromJSValue(const QJSValue &v);
        int style;
        QString color;

        ImageDesc texture;
    };

    struct PenDesc {
        static PenDesc fromJSValue(const QJSValue &v);

        BrushDesc brush;
        int width;
        int penStyle, capStyle, joinStyle;
    };

    static QBrush brush(QJSValue data, NivStore *store);
    static QBrush brush(Utils::BrushDesc desc, NivStore * store);
    static QPen pen(QJSValue data, NivStore * store);

};

class AbstractSceneItem : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSceneItem(QGraphicsItem * item, QObject * parent = nullptr);


public slots:
    void move(int x, int y);
    void setPos(int x, int y);
    void rotate(QJSValue r);
    void setOpacity(QJSValue v);
    QJSValue width();
    QJSValue height();
    void setZ(QJSValue v);
    void hide();
    void show();

    void hoverEnter();
    void hoverLeave();
    void hoverMove(int x, int y);

    void doubleClick(int x, int y);
    void mouseMove(int x, int y);
    void mousePress(int x, int y);
    void mouseRelease(int x, int y);

    void connectMouseEvents(QString type);
    void setMouseCallbacks(QJSValue cb) {mouseCallbacks = cb;}

protected:
    QGraphicsItem * item;
    QJSValue mouseCallbacks;
};

class SceneTextItem : public AbstractSceneItem
{
    Q_OBJECT
public:
    explicit SceneTextItem(QGraphicsTextItem* item, QObject *parent = nullptr);

    struct SceneTextItemDescriptor
    {
        static SceneTextItemDescriptor fromJSValue(const QJSValue& v);
        QFont font();
        QString text;
        QString fontFamily;
        QString fontColor;
        int fontSize;
        int fontWeight;
        bool italic;
        bool movable;
        bool selectable;
        bool fontIsSet;
    };

public slots:

    void setText(QString s);
    void setWidth(int width);
    QJSValue text();
    void setFont(QJSValue v);

signals:

};

class ScenePixmapItem : public AbstractSceneItem
{
    Q_OBJECT
public:
    explicit ScenePixmapItem(QGraphicsPixmapItem *item, QImage *img, QObject *parent = nullptr);
    ~ScenePixmapItem();

    struct ScenePixmapItemDescriptor
    {
        static ScenePixmapItemDescriptor fromJSValue(const QJSValue& v);
        QString filename;
        bool movable;
        bool selectable;
        bool cached;
    };

public slots:

    void resize(int w, int h);

signals:

private:
    QImage * img;
};


class SceneButtonItem : public AbstractSceneItem
{
    Q_OBJECT
public:
    explicit SceneButtonItem(QGraphicsProxyWidget *item, QPushButton*b, QObject *parent = nullptr);
    ~SceneButtonItem();

    struct SceneButtonItemDescriptor
    {
        static SceneButtonItemDescriptor fromJSValue(const QJSValue& v);
        QString text;
        QString img;
        QJSValue callback;
        bool movable;
        bool selectable;
    };

public slots:

    void setText(QString text);
    void setSize(int width, int height);
    void setImage(QString filename);
    void rescaleImage();

signals:

private:
    QPushButton * button;
    QImage * img;
};

class SceneVideoItem : public AbstractSceneItem
{
    Q_OBJECT
public:
    explicit SceneVideoItem(QGraphicsVideoItem *item, QMediaPlayer* mp, QByteArray * data, QBuffer * buf, QObject *parent = nullptr);
    ~SceneVideoItem();

    struct SceneVideoItemDescriptor
    {
        static SceneVideoItemDescriptor fromJSValue(const QJSValue& v);
        QString filename;
        int loops;
        double volume;
        bool movable;
        bool selectable;
    };

public slots:

    void setSize(int width, int height);
    void play();
    void pause();
    void stop();

signals:

private:
    QMediaPlayer * _mp;
    QByteArray * _data;
    QBuffer * _buf;
};

class SceneRectItem : public AbstractSceneItem
{
    Q_OBJECT
public:
    explicit SceneRectItem(QGraphicsRectItem *item, NivStore*store, QObject *parent = nullptr);
    ~SceneRectItem(){}

    struct SceneRectItemDescriptor
    {
        static SceneRectItemDescriptor fromJSValue(const QJSValue& v);
        bool movable;
        bool selectable;
        QString color;
        QString backgroundColor;
        QString backgroundImage;
        int width;
        int height;
    };
public slots:

    void setBrush(QJSValue v);
    void setPen(QJSValue v);
    void setRect(QJSValue v);

signals:

private:
    NivStore * store;
};


#endif // SCENEITEMS_H
