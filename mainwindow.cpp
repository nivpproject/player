#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJSEngine>
#include <QJSValue>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QFileDialog>
#include "sceneview.h"

MainWindow::MainWindow(QString n, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    engine = new QJSEngine(this);
    niv = new NivController(this);
    scene = new SceneView();
    scene->setEngine(engine);
    scene->show();

    QJSValue myScriptQObject = engine->newQObject(this->niv);
    engine->globalObject().setProperty("niv", myScriptQObject);
    QJSValue windowObject = engine->newQObject(this);
    engine->globalObject().setProperty("window", windowObject);

    QJSValue sceneObject = engine->newQObject(scene);
    engine->globalObject().setProperty("scene", sceneObject);

    QString filename = !n.isEmpty() ? n : QFileDialog::getOpenFileName(0, "", "", "Nivp Files (*.nivp)");
    if (!filename.isEmpty()) {
        QString script = scene->loadStoreFromFile(filename);
        QJSValue result = engine->evaluate(script);
        if (result.isError())
            qDebug()
                    << "Uncaught exception at line"
                    << result.property("lineNumber").toInt()
                    << ":" << result.toString();

        scene->activateWindow();
        this->showMinimized();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    engine->evaluate(ui->plainTextEdit->toPlainText());
}

QJSValue MainWindow::createButton(int x, int y, QString text, QJSValue onClick, QJSValue id)
{
    QPushButton * b = new QPushButton(this);
    b->setText(text);
    b->move(x,y);
        if (onClick.isCallable())
        connect(b, &QPushButton::clicked, [onClick](){
            onClick.call();
        });
    if (!id.isUndefined()) {
        widgets[id.toString()] = b;
    }
    return engine->newQObject(b);
}

QJSValue MainWindow::addWidgetToScene(QString id)
{
    if (widgets.contains(id))
    return scene->addWidget(widgets[id]);
    return QJSValue();
}

void MainWindow::playVideo(QString filename)
{
    QFile f(filename);
    if (f.open(QFile::ReadOnly)){
        QByteArray *data = new QByteArray(f.readAll());
        QBuffer *buf = new QBuffer(data);
        buf->open(QBuffer::ReadOnly);
        QMediaPlayer *mp = new QMediaPlayer();
        QVideoWidget *vw = new QVideoWidget(scene);
        //scene->addWidget(vw);
        vw->show();
        mp->setVideoOutput(vw);
        mp->setSourceDevice(buf, QUrl(filename));
        mp->play();
    }
}
