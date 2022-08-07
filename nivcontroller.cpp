#include "nivcontroller.h"
#include <QDebug>
#include <QFileDialog>
#include <QColor>

NivController::NivController(QObject *parent)
    : QObject{parent}
{
    engine = 0;
}

void NivController::loadImage(QString s)
{
    qDebug() << s;
}

void NivController::loadFileDialog()
{
    QString s = QFileDialog::getOpenFileName();
    if (!s.isEmpty())
        emit drawImage(s);
}

void NivController::debug(QString s)
{
    qDebug() << s;
}

QJSValue NivController::rgba(int r, int g, int b, int a)
{
    return QJSValue(QColor::fromRgb(r,g,b,a).name());
}

QJSValue NivController::__img()
{
    return getFileNameDialog(filters("images").toString());
}

QJSValue NivController::getFileNameDialog(QString filter)
{
    QString s = QFileDialog::getOpenFileName(0,"","",filter);
    if (!s.isEmpty())
        return QJSValue(s);
    return QJSValue();
}

QJSValue NivController::filters(QString name)
{
    if (name.toLower() == "images")
        return QJSValue(QString("Image Files (*.png *.jpg *.bmp)"));
    return QJSValue();
}
