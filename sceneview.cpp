#include "sceneview.h"
#include "ui_sceneview.h"
#include <QJSValue>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
#include <QPushButton>
#include <QIcon>
#include <QOpenGLWidget>
#include <QGraphicsVideoItem>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QPen>
#include <QFileDialog>
#include <QKeySequence>
#include <QMargins>
#include "sceneitems.h"
#include "graphicitems.h"

SceneView::SceneView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SceneView)
{
    ui->setupUi(this);
    lockedScale = false;

    scene = new QGraphicsScene(this);
    view = new GraphicsView(this);
    this->setWindowTitle("Project");
    view->show();
    ui->gridLayout->addWidget(view, 0,0);

    view->setScene(scene);
    view->setBackgroundBrush(QBrush(QColor("black")));
    view->setViewport(new QOpenGLWidget());
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setSceneRect(0,0,view->width(), view->height());
    view->setMouseTracking(true);

    connect(view, &GraphicsView::keyEvent, this, &SceneView::keyEvent);
    connect(view, &GraphicsView::mouseMoveSignal, this, &SceneView::mouseEvent);

    this->setStyleSheet("border: 0px");
}

SceneView::~SceneView()
{
    delete ui;
}

QJSValue SceneView::addWidget(QWidget *w)
{
    auto obj = scene->addWidget(w);

    auto item = new AbstractSceneItem(obj, this);
    return engine->newQObject(item);
}

void SceneView::resizeEvent(QResizeEvent *event)
{
    view->centerOn(this->size().width()/2, this->size().height()/2);
    if (event->oldSize().isValid() && this->lockedScale) {
         double ws = (this->size().width() * 1.0) / (event->oldSize().width() * 1.0);
         double hs = (this->size().height() * 1.0) / (event->oldSize().height() * 1.0);
         view->scale(ws, hs);
    }
}

QString SceneView::loadStoreFromFile(QString &filename)
{

    storeFile = NivBuilderFile::fromFile(filename);
    store.filename = filename;
    store.items.clear();
    store.src = &storeFile;
    this->setWindowTitle(storeFile.name);
    return storeFile.script;
}

QImage *SceneView::loadImage(QString filename, bool useCache)
{
    QImage *img = 0;
    if (useCache && imageCache.contains(filename))
        img = imageCache[filename];
    else if (store.items.contains(filename)) {
        img = new QImage(QImage::fromData(store.items[filename].data));
    }
    else img = new QImage(filename);
    return img;
}

QImage SceneView::loadImageTemp(QString filename)
{
    if (store.items.contains(filename))
        return QImage(QImage::fromData(store.items[filename].data));
    return QImage(filename);
}

void SceneView::closeEvent(QCloseEvent *)
{
    qApp->exit();
}

QJSValue SceneView::addText(QJSValue data)
{
    auto desc = SceneTextItem::SceneTextItemDescriptor::fromJSValue(data);
    auto obj = new GraphicsTextItem();
    obj->setPlainText(desc.text);
    obj->setFont(desc.font());

    scene->addItem(obj);
    if (!desc.fontColor.isEmpty()) {
        QColor c(desc.fontColor);
        obj->setDefaultTextColor(c);
    }
    if (desc.movable)
        obj->setFlag(QGraphicsItem::ItemIsMovable);
    if (desc.selectable)
        obj->setFlag(QGraphicsItem::ItemIsSelectable);

    auto item = new SceneTextItem(obj,this);
    item->connectMouseEvents("text");
    objToItems[obj] = item;
    return engine->newQObject(item);
}

QJSValue SceneView::addImage(QJSValue data)
{
    auto desc = ScenePixmapItem::ScenePixmapItemDescriptor::fromJSValue(data);

    QImage *img = loadImage(desc.filename);

    if (img && !img->isNull()) {
        auto obj = new GraphicsPixmapItem();
        obj->setPixmap(QPixmap::fromImage(*img));
        scene->addItem(obj);
        if (desc.movable)
            obj->setFlag(QGraphicsItem::ItemIsMovable);
        if (desc.selectable)
            obj->setFlag(QGraphicsItem::ItemIsSelectable);
        auto item = new ScenePixmapItem(obj,img, this);
        item->connectMouseEvents("image");
        objToItems[obj] = item;
        return engine->newQObject(item);
    }
    return QJSValue();
}

QJSValue SceneView::addButton(QJSValue data)
{
    auto desc = SceneButtonItem::SceneButtonItemDescriptor::fromJSValue(data);
    QPushButton * b = new QPushButton();
    b->setText(desc.text);
    if (desc.callback.isCallable())
        connect(b, &QPushButton::clicked, [desc](){
            desc.callback.call();
        });
    if (!desc.img.isEmpty()) {
        QImage img(desc.img);
        QPixmap pm = QPixmap::fromImage(img.scaled(b->width(),b->height(), Qt::KeepAspectRatio));
        QIcon icon(pm);
        b->setIcon(icon);
        b->setIconSize(QSize(b->width(), b->height()));
    }
    auto obj = scene->addWidget(b);
    if (desc.movable)
        obj->setFlag(QGraphicsItem::ItemIsMovable);
    if (desc.selectable)
        obj->setFlag(QGraphicsItem::ItemIsSelectable);
    auto item = new SceneButtonItem(obj, b, this);
    objToItems[obj] = item;
    return engine->newQObject(item);
}

QJSValue SceneView::addVideo(QJSValue data)
{
    auto desc = SceneVideoItem::SceneVideoItemDescriptor::fromJSValue(data);

    QByteArray *badata = 0;
    if (store.items.contains(desc.filename)) {
        badata = new QByteArray(store.items[desc.filename].data);
    }
    else {

        QFile f(desc.filename);
        if (f.open(QFile::ReadOnly)){
            badata = new QByteArray(f.readAll());
            f.close();
        }
    }

    if (badata && badata->length()) {
        QBuffer *buf = new QBuffer(badata);
        buf->open(QBuffer::ReadOnly);
        QMediaPlayer *mp = new QMediaPlayer();
        QGraphicsVideoItem *vi = new QGraphicsVideoItem();
        mp->setVideoOutput(vi);
        mp->setLoops(desc.loops);
        if (desc.movable)
            vi->setFlag(QGraphicsItem::ItemIsMovable);
        if (desc.selectable)
            vi->setFlag(QGraphicsItem::ItemIsSelectable);

        scene->addItem(vi);
        mp->setSourceDevice(buf, QUrl(desc.filename));

        if (desc.volume != -1 && mp->hasAudio()) {
            auto aOutput = new QAudioOutput(this);
            mp->setAudioOutput(aOutput);
            aOutput->setVolume(desc.volume);
        }

        auto item = new SceneVideoItem(vi, mp, badata, buf, this);
        objToItems[vi] = item;
        return engine->newQObject(item);
    }

    return QJSValue();
}

QJSValue SceneView::addRect(QJSValue data)
{
    auto desc = SceneRectItem::SceneRectItemDescriptor::fromJSValue(data);
    auto obj = new GraphicsRectItem();
    obj->setRect(0,0, desc.width, desc.height);
    obj->setBrush(QBrush(QColor(desc.backgroundColor)));
    if (!desc.backgroundImage.isEmpty()) {
        auto img = loadImageTemp(desc.backgroundImage);
        obj->setBrush(QBrush(img));
    }
    obj->setPen(QPen(QColor(desc.color)));

    scene->addItem(obj);

    if (desc.movable)
        obj->setFlag(QGraphicsItem::ItemIsMovable);
    if (desc.selectable)
        obj->setFlag(QGraphicsItem::ItemIsSelectable);

    auto item = new SceneRectItem(obj, &store, this);
    item->connectMouseEvents("rect");
    objToItems[obj] = item;
    return engine->newQObject(item);
}

QJSValue SceneView::createTimer(int interval, QJSValue callback)
{
    QTimer * t = new QTimer(this);
    t->setInterval(interval);
    connect(t, &QTimer::timeout, [callback](){
        callback.call();
    });
    return engine->newQObject(t);
}

void SceneView::delay(int interval, QJSValue callback)
{
    QTimer * t = new QTimer(this);
    t->setInterval(interval);
    connect(t, &QTimer::timeout, [t, callback](){
        callback.call();
        t->stop();
    });
    t->start();
}

void SceneView::installKeyHandler(QString id, QJSValue callback)
{
    if (callback.isCallable())
        keyCallbacks[id] = callback;
}

void SceneView::installMouseMoveHandler(QString id, QJSValue callback)
{
    if (callback.isCallable())
        mouseMoveCallbacks[id] = callback;
}

void SceneView::uninstallKeyHandler(QString id)
{
    keyCallbacks.remove(id);
}

void SceneView::uninstallMouseMoveHandler(QString id)
{
    mouseMoveCallbacks.remove(id);
}

void SceneView::keyEvent(QString k, bool up)
{
    foreach (const QString &s, keyCallbacks.keys()) {
        if (keyCallbacks[s].isCallable()) {
            keyCallbacks[s].call(QJSValueList() << k << up);
        }
    }
}

void SceneView::mouseEvent(QMouseEvent *e)
{
    foreach (const QString &s, mouseMoveCallbacks.keys()) {
        if (mouseMoveCallbacks[s].isCallable()) {
            mouseMoveCallbacks[s].call(QJSValueList() << e->pos().x() << e->pos().y());
        }
    }
}

void SceneView::resizeWindow(int w, int h)
{
    this->resize(w, h);
}

void SceneView::show(QString type)
{
    if (type == "normal")
        this->showNormal();
    else if (type == "maximized")
        this->showMaximized();
    else if (type == "fullscreen")
        this->showFullScreen();
}

QJSValue SceneView::windowSize()
{
    QJSValue result = engine->newObject();
    result.setProperty("width",QJSValue(this->width()));
    result.setProperty("height", QJSValue(this->height()));
    return result;
}


void SceneView::resetScale()
{
    view->resetTransform();
}

void SceneView::setCameraRect(int x, int y, int w, int h)
{
    view->setSceneRect(x, y, w, h);
}

void SceneView::loadPartition(QString name)
{
    store.loadPartition(name);
}

void SceneView::unloadPartition(QString name)
{
    store.unloadPartition(name);
}

void SceneView::setBackgroundBrush(QJSValue data)
{
    view->setBackgroundBrush(Utils::brush(data, &store));
}

void SceneView::cacheImage(QString filename, QString id)
{
    if (!filename.isEmpty()){
        QImage *img = loadImage(filename, false);
        if (!id.isEmpty()) {
            imageCache[id] = img;
        } else imageCache[filename] = img;
    }
}


QFont SceneView::fontFromJS(QJSValue v)
{
    if (v.isUndefined())
        return QFont();
    return QFont(v.property("fontFamily").toString(),
                 v.property("fontSize").toInt(),
                 v.property("fontWeight").toInt(),
                 v.property("italic").toBool());
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    emit mouseMoveSignal(e);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyEvent(keyToString(event), false);
        qDebug() << "pressed " << keyToString(event);
    }
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        emit keyEvent(keyToString(event), true);
        qDebug() << "released " << keyToString(event);
    }
}

QString GraphicsView::keyToString(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Shift:
        return "SHIFT";
    case Qt::Key_Alt:
        return "ALT";
    case Qt::Key_Control:
        return "CTRL";
    default:
        return QKeySequence(event->key()).toString();
    }
}

