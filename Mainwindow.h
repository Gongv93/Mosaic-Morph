

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Tile.h"
#include "glwidget.h"
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>

namespace Ui
{
class MainWindow;
class QAction;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     //Constructor
     MainWindow();

     //Desttructor
    ~MainWindow();

public slots:

    /*******  FILE  ********/
    void    s_loadTiles   ();
    void    s_save        ();
    void    s_saveAs      ();
    void    s_quit        ();
    void    s_SetCentroid();
    void    s_SetRotate();
    void    s_SetScale();

private:
     //****Create Menu****
     void createMenuFile ();

     //****Create Actions****
     void createActionsFile ();

     //***Layout for Left Side***
     void leftLayout();

     //****QAction for File****
     QAction*     m_actionLoadTiles;
     QAction*     m_actionSave;
     QAction*     m_actionSaveAs;
     QAction*     m_actionQuit;
     QAction*     m_actionCentroid;

     //****QMenu****
     QMenu* m_menuFile;

     //Buttons
     QPushButton *m_LoadButton;
     QPushButton *m_playPause;

     //Check Boxes
     QCheckBox   *m_scaleTiles;
     QCheckBox   *m_rotateTiles;
     QCheckBox   *m_showCent;


     vector<Tile>	 m_tiles;	  // array of mosaic tiles
     GLWidget	    *m_glwidget; // widget in which to draw



};

#endif // MAINWINDOW_H
