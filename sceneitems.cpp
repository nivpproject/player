#include "sceneitems.h"
#include <QJSEngine>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <graphicitems.h>

QBrush Utils::brush(QJSValue data, NivStore * store)
{
    return brush(Utils::BrushDesc::fromJSValue(data), store);
}

QBrush Utils::brush(Utils::BrushDesc desc, NivStore * store)
{
    if (desc.style == -1) {
        QImage img = QImage();
        if (store->items.contains(desc.texture.filename))
            img = QImage::fromData(store->items[desc.texture.filename].data);
        else img = QImage(desc.texture.filename);

        if (!img.isNull()) {
            if (desc.texture.width && desc.texture.height)
                return QBrush(img.scaled(desc.texture.width, desc.texture.height));
            else
                return QBrush(img);
        }
    }
    return QBrush(QColor(desc.color), (Qt::BrushStyle) desc.style);
}

QPen Utils::pen(QJSValue data, NivStore * store)
{
    Utils::PenDesc desc = Utils::PenDesc::fromJSValue(data);
    return QPen(brush(desc.brush, store), desc.width,
                (Qt::PenStyle) desc.penStyle,
                (Qt::PenCapStyle) desc.capStyle,
                (Qt::PenJoinStyle) desc.joinStyle);
}

Utils::ImageDesc Utils::ImageDesc::fromJSValue(const QJSValue &v)
{
    Utils::ImageDesc result;
    result.filename = v.property("filename").toString();
    result.width = v.hasProperty("width") ? v.property("width").toInt() : 0;
    result.height = v.hasProperty("height") ? v.property("height").toInt() : 0;
    return result;
}

Utils::BrushDesc Utils::BrushDesc::fromJSValue(const QJSValue &v)
{
    Utils::BrushDesc result;
    QString style = v.property("style").toString();
    if (!v.hasProperty("style") || style == "SolidPattern")
        result.style = Qt::SolidPattern;
    else if (style == "NoBrush")
        result.style = Qt::NoBrush;
    else if (style == "Dense1Pattern")
        result.style = Qt::Dense1Pattern;
    else if (style == "Dense2Pattern")
        result.style = Qt::Dense2Pattern;
    else if (style == "Dense3Pattern")
        result.style = Qt::Dense3Pattern;
    else if (style == "Dense4Pattern")
        result.style = Qt::Dense4Pattern;
    else if (style == "Dense5Pattern")
        result.style = Qt::Dense5Pattern;
    else if (style == "Dense6Pattern")
        result.style = Qt::Dense6Pattern;
    else if (style == "Dense7Pattern")
        result.style = Qt::Dense7Pattern;

    else if (style == "HorPattern")
        result.style = Qt::HorPattern;
    else if (style == "VerPattern")
        result.style = Qt::VerPattern;
    else if (style == "CrossPattern")
        result.style = Qt::CrossPattern;
    else if (style == "BDiagPattern")
        result.style = Qt::BDiagPattern;
    else if (style == "FDiagPattern")
        result.style = Qt::FDiagPattern;
    else if (style == "DiagCrossPattern")
        result.style = Qt::DiagCrossPattern;

    else if (style == "texture") {
        result.style = -1;
        result.texture = ImageDesc::fromJSValue(v.property("texture"));
    }
    result.color = v.property("color").toString();
    return result;
}

Utils::PenDesc Utils::PenDesc::fromJSValue(const QJSValue &v)
{
    PenDesc result;
    result.brush = BrushDesc::fromJSValue(v.property("brush"));
    QString penStyle = v.hasProperty("penStyle") ? v.property("penStyle").toString()
                                                 : "SolidLine";
    QString capStyle = v.hasProperty("capStyle") ? v.property("capStyle").toString()
                                                 : "SquareCap";
    QString joinStyle = v.hasProperty("joinStyle") ? v.property("joinStyle").toString()
                                                   : "BevelJoin";

    if (penStyle == "SolidLine")
        result.penStyle = Qt::SolidLine;
    if (penStyle == "DashLine")
        result.penStyle = Qt::DashLine;
    if (penStyle == "DotLine")
        result.penStyle = Qt::DotLine;
    if (penStyle == "DashDotLine")
        result.penStyle = Qt::DashDotLine;
    if (penStyle == "DashDotDotLine")
        result.penStyle = Qt::DashDotDotLine;

    if (capStyle == "SquareCap")
        result.capStyle = Qt::SquareCap;
    if (capStyle == "FlatCap")
        result.capStyle = Qt::FlatCap;
    if (capStyle == "RoundCap")
        result.capStyle = Qt::RoundCap;

    if (joinStyle == "BevelJoin")
        result.joinStyle = Qt::BevelJoin;
    if (joinStyle == "MiterJoin")
        result.joinStyle = Qt::MiterJoin;
    if (joinStyle == "RoundJoin")
        result.joinStyle = Qt::RoundJoin;

    result.width = v.property("width").toInt();

    return result;

}



SceneTextItem::SceneTextItem(QGraphicsTextItem *item, QObject *parent) : AbstractSceneItem(item, parent)
{
}

void SceneTextItem::setText(QString s)
{
    dynamic_cast<QGraphicsTextItem*>(item)->setPlainText(s);
}

void SceneTextItem::setWidth(int width)
{
    dynamic_cast<QGraphicsTextItem*>(item)->setTextWidth(width);
}

QJSValue SceneTextItem::text()
{
    return QJSValue(dynamic_cast<QGraphicsTextItem*>(item)->toPlainText());
}

void SceneTextItem::setFont(QJSValue v)
{
    auto item_ = dynamic_cast<QGraphicsTextItem*>(item);
    QFont f = item_->font();
    if (!v.property("family").isUndefined())
        f.setFamily(v.property("family").toString());
    if (!v.property("size").isUndefined())
        f.setPointSize(v.property("size").toInt());
    if (!v.property("italic").isUndefined())
        f.setItalic(v.property("italic").toBool());
    item_->setFont(f);
    if (!v.property("color").isUndefined())
        item_->setDefaultTextColor(QColor(v.property("color").toString()));
}

SceneTextItem::SceneTextItemDescriptor SceneTextItem::SceneTextItemDescriptor::fromJSValue(const QJSValue &v)
{
    SceneTextItemDescriptor result;
    result.text = v.property("text").toString();
    result.movable = v.property("movable").toBool();
    result.selectable = v.property("selectable").toBool();

    auto font = v.property("font");
    if (!font.isUndefined()) {
        result.fontFamily = font.property("family").toString();
        result.fontSize = font.property("size").toInt();
        result.fontWeight = font.property("weight").toInt();
        result.italic = font.property("italic").toBool();
        result.fontColor = font.property("color").toString();
        result.fontIsSet = true;
    } else result.fontIsSet = false;

    return result;
}

QFont SceneTextItem::SceneTextItemDescriptor::font()
{
    if (fontIsSet) {
        return QFont(fontFamily, fontSize, fontWeight, italic);
    }
    return QFont();
}


ScenePixmapItem::ScenePixmapItemDescriptor ScenePixmapItem::ScenePixmapItemDescriptor::fromJSValue(const QJSValue &v)
{
    ScenePixmapItemDescriptor result;
    result.filename = v.property("filename").toString();
    result.movable = v.property("movable").toBool();
    result.selectable = v.property("selectable").toBool();
    result.cached = v.property("cached").toBool();
    return result;
}

ScenePixmapItem::ScenePixmapItem(QGraphicsPixmapItem *item, QImage *img, QObject *parent): AbstractSceneItem(item, parent)
{
    this->img = img;
}

ScenePixmapItem::~ScenePixmapItem()
{

}


void ScenePixmapItem::resize(int w, int h)
{
    auto newImage = img->scaled(w, h, Qt::KeepAspectRatio);
    dynamic_cast<QGraphicsPixmapItem*>(item)->setPixmap(QPixmap::fromImage(newImage));
}

AbstractSceneItem::AbstractSceneItem(QGraphicsItem *item, QObject *parent): QObject{parent}
{
    this->item = item;
}

void AbstractSceneItem::connectMouseEvents(QString type)
{
    if (type == "text") {
        auto txt = dynamic_cast<GraphicsTextItem*>(item);
        connect(txt, &GraphicsTextItem::hoverEnter, this, &AbstractSceneItem::hoverEnter);
        connect(txt, &GraphicsTextItem::hoverLeave, this, &AbstractSceneItem::hoverLeave);
        connect(txt, &GraphicsTextItem::hoverMove, this, &AbstractSceneItem::hoverMove);

        connect(txt, &GraphicsTextItem::doubleClick, this, &AbstractSceneItem::doubleClick);
        connect(txt, &GraphicsTextItem::mouseMove, this, &AbstractSceneItem::mouseMove);
        connect(txt, &GraphicsTextItem::mousePress, this, &AbstractSceneItem::mousePress);
        connect(txt, &GraphicsTextItem::mouseRelease, this, &AbstractSceneItem::mouseRelease);
    } else if (type == "image") {
        auto img = dynamic_cast<GraphicsPixmapItem*>(item);
        connect(img, &GraphicsPixmapItem::hoverEnter, this, &AbstractSceneItem::hoverEnter);
        connect(img, &GraphicsPixmapItem::hoverLeave, this, &AbstractSceneItem::hoverLeave);
        connect(img, &GraphicsPixmapItem::hoverMove, this, &AbstractSceneItem::hoverMove);

        connect(img, &GraphicsPixmapItem::doubleClick, this, &AbstractSceneItem::doubleClick);
        connect(img, &GraphicsPixmapItem::mouseMove, this, &AbstractSceneItem::mouseMove);
        connect(img, &GraphicsPixmapItem::mousePress, this, &AbstractSceneItem::mousePress);
        connect(img, &GraphicsPixmapItem::mouseRelease, this, &AbstractSceneItem::mouseRelease);
    } else if (type == "rect") {
        auto r = dynamic_cast<GraphicsRectItem*>(item);
        connect(r, &GraphicsRectItem::hoverEnter, this, &AbstractSceneItem::hoverEnter);
        connect(r, &GraphicsRectItem::hoverLeave, this, &AbstractSceneItem::hoverLeave);
        connect(r, &GraphicsRectItem::hoverMove, this, &AbstractSceneItem::hoverMove);

        connect(r, &GraphicsRectItem::doubleClick, this, &AbstractSceneItem::doubleClick);
        connect(r, &GraphicsRectItem::mouseMove, this, &AbstractSceneItem::mouseMove);
        connect(r, &GraphicsRectItem::mousePress, this, &AbstractSceneItem::mousePress);
        connect(r, &GraphicsRectItem::mouseRelease, this, &AbstractSceneItem::mouseRelease);
    }
}

void AbstractSceneItem::move(int x, int y)
{
    item->moveBy(x, y);
}

void AbstractSceneItem::setPos(int x, int y)
{
    item->setPos(x, y);
}

void AbstractSceneItem::rotate(QJSValue r)
{
    auto rect = item->boundingRect();
    QTransform tr;
    tr.translate(rect.center().rx(), rect.center().ry());
    tr.rotate(r.toNumber());
    tr.translate(-rect.center().rx(), -rect.center().ry());
    item->setTransform(tr);
}

void AbstractSceneItem::setOpacity(QJSValue v)
{
    item->setOpacity(v.toNumber());
}

QJSValue AbstractSceneItem::width()
{
    return QJSValue(item->boundingRect().width());
}

QJSValue AbstractSceneItem::height()
{
    return QJSValue(item->boundingRect().height());
}

void AbstractSceneItem::setZ(QJSValue v)
{
    item->setZValue(v.toNumber());
}

void AbstractSceneItem::hide()
{
    item->hide();
}

void AbstractSceneItem::show()
{
    item->show();
}

void AbstractSceneItem::hoverEnter()
{
    if (mouseCallbacks.property("hoverEnter").isCallable()) {
        mouseCallbacks.property("hoverEnter").call();
    }
}

void AbstractSceneItem::hoverLeave()
{
    if (mouseCallbacks.property("hoverLeave").isCallable()) {
        mouseCallbacks.property("hoverLeave").call();
    }
}

void AbstractSceneItem::hoverMove(int x, int y)
{
    if (mouseCallbacks.property("hoverMove").isCallable()) {
        mouseCallbacks.property("hoverMove").call(QJSValueList() << x << y);
    }
}

void AbstractSceneItem::doubleClick(int x, int y)
{
    if (mouseCallbacks.property("doubleClick").isCallable()) {
        mouseCallbacks.property("doubleClick").call(QJSValueList() << x << y);
    }
}

void AbstractSceneItem::mouseMove(int x, int y)
{
    if (mouseCallbacks.property("mouseMove").isCallable()) {
        mouseCallbacks.property("mouseMove").call(QJSValueList() << x << y);
    }
}

void AbstractSceneItem::mousePress(int x, int y)
{
    if (mouseCallbacks.property("mousePress").isCallable()) {
        mouseCallbacks.property("mousePress").call(QJSValueList() << x << y);
    }
}

void AbstractSceneItem::mouseRelease(int x, int y)
{
    if (mouseCallbacks.property("mouseRelease").isCallable()) {
        mouseCallbacks.property("mouseRelease").call(QJSValueList() << x << y);
    }
}

SceneButtonItem::SceneButtonItem(QGraphicsProxyWidget *item, QPushButton *b, QObject *parent) : AbstractSceneItem(item, parent)
{
    this->button = b;
    this->img = 0;
}

SceneButtonItem::~SceneButtonItem()
{
}

void SceneButtonItem::setText(QString text)
{
    if (button)
        button->setText(text);
}

void SceneButtonItem::setSize(int width, int height)
{
    if (button)
        button->setFixedSize(width, height);
}

void SceneButtonItem::setImage(QString filename)
{
    if (!filename.isEmpty() && button) {
        this->img = new QImage(filename);
        QPixmap pm = QPixmap::fromImage(img->scaled(button->width(),button->height()));
        QIcon icon(pm);
        button->setIcon(icon);
        button->setIconSize(QSize(button->width(), button->height()));
    }
}

void SceneButtonItem::rescaleImage()
{
    if (img) {
        QPixmap pm = QPixmap::fromImage(img->scaled(button->width(),button->height()));
        QIcon icon(pm);
        button->setIcon(icon);
        button->setIconSize(QSize(button->width(), button->height()));
    }
}

SceneButtonItem::SceneButtonItemDescriptor SceneButtonItem::SceneButtonItemDescriptor::fromJSValue(const QJSValue &v)
{
    SceneButtonItemDescriptor result;
    result.text = v.property("text").toString();
    result.img = v.property("img").toString();
    result.movable = v.property("movable").toBool();
    result.selectable = v.property("selectable").toBool();
    result.callback = v.property("onClick");
    return result;
}

SceneVideoItem::SceneVideoItem(QGraphicsVideoItem *item, QMediaPlayer *mp, QByteArray *data, QBuffer *buf, QObject *parent)
    :AbstractSceneItem(item, parent)
{
    _mp = mp;
    _data = data;
    _buf = buf;
}

SceneVideoItem::~SceneVideoItem()
{
    _mp->stop();
    delete _mp;
    delete _buf;
    delete _data;
}

void SceneVideoItem::setSize(int width, int height)
{
    dynamic_cast<QGraphicsVideoItem*>(item)->setSize(QSize(width, height));
}

void SceneVideoItem::play()
{
    _mp->play();
}

void SceneVideoItem::pause()
{
    _mp->pause();
}

void SceneVideoItem::stop()
{
    _mp->stop();
}

SceneVideoItem::SceneVideoItemDescriptor SceneVideoItem::SceneVideoItemDescriptor::fromJSValue(const QJSValue &v)
{
    SceneVideoItemDescriptor result;
    result.filename = v.property("filename").toString();
    if (v.hasProperty("loops"))
        result.loops = v.property("loops").toNumber();
    else
        result.loops = -1;
    if (v.hasProperty("volume"))
        result.volume = v.property("volume").toNumber();
    else
        result.volume = -1;
    result.movable = v.property("movable").toBool();
    result.selectable = v.property("selectable").toBool();
    return result;
}

SceneRectItem::SceneRectItemDescriptor SceneRectItem::SceneRectItemDescriptor::fromJSValue(const QJSValue &v)
{
    SceneRectItem::SceneRectItemDescriptor result;
    result.color = v.property("color").toString();
    result.backgroundColor = v.property("backgroundColor").toString();
    result.backgroundImage = v.property("backgroundImage").isUndefined() ? "" : v.property("backgroundImage").toString();
    result.movable = v.property("movable").toBool();
    result.selectable = v.property("selectable").toBool();
    result.width = v.property("width").toNumber();
    result.height = v.property("height").toNumber();
    return result;
}

SceneRectItem::SceneRectItem(QGraphicsRectItem *item, NivStore *store, QObject *parent) : AbstractSceneItem(item, parent)
{
    this->store = store;
}

void SceneRectItem::setBrush(QJSValue v)
{
    dynamic_cast<QGraphicsRectItem*>(item)->setBrush(Utils::brush(v, store));
}

void SceneRectItem::setPen(QJSValue v)
{
    dynamic_cast<QGraphicsRectItem*>(item)->setPen(Utils::pen(v, store));
}

void SceneRectItem::setRect(QJSValue v)
{
    dynamic_cast<QGraphicsRectItem*>(item)->setRect(v.property("x").toInt(),
                                                    v.property("y").toInt(),
                                                    v.property("width").toInt(),
                                                    v.property("height").toInt());
}

