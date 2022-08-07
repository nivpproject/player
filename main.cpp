#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("source", "Project file name");
    parser.process(a);
    if (parser.positionalArguments().length()) {
        QString name = parser.positionalArguments()[0];
        MainWindow w(name);
        w.show();
        return a.exec();
    }
    else {
        MainWindow w("");
        w.show();
        return a.exec();
    }
    return a.exec();
}
