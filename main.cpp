/* ========================================================================
   File:    main.cpp
   Class:   Senior Design Csc 59866
   Creator: Asad Kamal, Vincent Gong, RuLong Haung
   ======================================================================== */

/*
    MOSAIC_VERSION
     0 - Build for tile morph
     1 - Build for explosion
*/

#include "Mainwindow.h"
#include <QtWidgets>
#include <QApplication>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// main:
//
// Main function where it all begins.
//

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(900,750);
    window.show();

    return app.exec();
}
