#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "sceneitems.h"
#include <QJSEngine>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>
#include <QHash>
#include <QImage>
#include <QMouseEvent>
#include <QEvent>
#include <QKeyEvent>
#include "niv.h"

namespace Ui {
class SceneView;
}

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget * parent) : QGraphicsView(parent){lastItem = 0;}
    void mouseMoveEvent(QMouseEvent * e);

    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

    QString keyToString(QKeyEvent * event);


signals:
    void onHoverEnter(QGraphicsItem * item, int x, int y);
    void onHoverLeave(QGraphicsItem * item);
    void keyEvent(QString key, bool up);
    void mouseMoveSignal(QMouseEvent * e);

private:
    QGraphicsItem * lastItem;
};

class SceneView : public QWidget
{
    Q_OBJECT

public:
    explicit SceneView(QWidget *parent = nullptr);
    QGraphicsScene * getScene() { return scene; }
    ~SceneView();
    void setEngine(QJSEngine *e) {engine = e;};
    QJSValue addWidget(QWidget* w);
    void resizeEvent(QResizeEvent* event);

    QString loadStoreFromFile(QString &filename);
    QImage* loadImage(QString filename, bool useCache = true);
    QImage loadImageTemp(QString filename);

    void closeEvent(QCloseEvent *);


public slots:


    QJSValue addText(QJSValue data);
    QJSValue addImage(QJSValue data);
    QJSValue addButton(QJSValue data);
    QJSValue addVideo(QJSValue data);
    QJSValue addRect(QJSValue data);
    QJSValue createTimer(int interval, QJSValue callback);
    void delay(int interval, QJSValue callback);
    void later(QJSValue callback) {delay(1, callback);}

    void installKeyHandler(QString id, QJSValue callback);
    void installMouseMoveHandler(QString id, QJSValue callback);
    void uninstallKeyHandler(QString id);
    void uninstallMouseMoveHandler(QString id);
    void keyEvent(QString k, bool up);
    void mouseEvent(QMouseEvent * e);


    void resizeWindow(int w, int h);
    void show(QString type = "normal");
    QJSValue windowSize();
    void lockScale() {lockedScale = true;}
    void unlockScale() {lockedScale = false;}
    void resetScale();
    void setCameraRect(int x, int y, int w, int h);

    void loadPartition(QString name);
    void unloadPartition(QString name);

    void setBackgroundBrush(QJSValue data);


    void cacheImage(QString filename, QString id);
    void cacheImage(QString filename) {cacheImage(filename, "");}

private:
    QFont fontFromJS(QJSValue v);

    Ui::SceneView *ui;

    QGraphicsScene * scene;
    GraphicsView * view;
    QJSEngine * engine;

    NivBuilderFile storeFile;
    NivStore store;

    bool lockedScale;
    QHash<QString, QImage*> imageCache;
    QHash<QGraphicsItem*, AbstractSceneItem*> objToItems;
    QHash<QString, QJSValue> keyCallbacks;
    QHash<QString, QJSValue> mouseMoveCallbacks;
};

#endif // SCENEVIEW_H

/*
 *
var img = scene.addImage({filename: niv.__img()});
scene.unlockScale();
scene.uninstallMouseMoveHandler("main");

let s = 40.0;

scene.installMouseMoveHandler("main", (x, y) => {
    let ws = scene.windowSize();
    let diffx = ws.width/2.0/s;
    let diffy = ws.height/2.0/s;
    img.setPos((ws.width / 2 - x)/diffx-s, (ws.height / 2 - y)/diffy-s);
    img.rotate((ws.width/2 - x)/(ws.width/2)*5);
});
scene.resizeWindow(1200, 675);

scene.lockScale();

scene.later(() => {scene.show("fullscreen");});
 * */
