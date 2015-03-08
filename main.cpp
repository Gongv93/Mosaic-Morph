#include "Mainwindow.h"
#include <QtWidgets>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(900,750);
    window.show();

    return app.exec();
}
