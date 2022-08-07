#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nivcontroller.h"
#include "sceneview.h"
#include <QJSEngine>
#include <QJSValue>
#include <QWidget>
#include <QHash>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString n, QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void on_pushButton_clicked();
    QJSValue createButton(int x, int y, QString text, QJSValue onClick, QJSValue id);
    QJSValue addWidgetToScene(QString id);

    void playVideo(QString filename);

private slots:

private:
    Ui::MainWindow *ui;
    NivController *niv;
    QJSEngine *engine;
    SceneView * scene;
    QHash<QString, QWidget*> widgets;

};
#endif // MAINWINDOW_H
