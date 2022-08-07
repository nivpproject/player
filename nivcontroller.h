#ifndef NIVCONTROLLER_H
#define NIVCONTROLLER_H

#include <QObject>
#include <QJSEngine>

class NivController : public QObject
{
    Q_OBJECT
public:
    explicit NivController(QObject *parent = nullptr);
    void setEngine(QJSEngine *e);

public slots:
    void loadImage(QString s);
    void loadFileDialog();
    void debug(QString s);
    QJSValue rgba(int r, int g, int b, int a = 255);


    QJSValue __img();
    QJSValue getFileNameDialog(QString filter);
    QJSValue filters(QString name);

signals:
    void drawImage(QString s);
private:
    QJSEngine * engine;
};

#endif // NIVCONTROLLER_H
