
/********** Senior Design Csc 59866 Section:
 ********** Asad Kamal     Email:
 ********** Vincent Gong   Email:
 ********** RuLong Huang   Email:
*/

#include "Mainwindow.h"


MainWindow::MainWindow()
{
    createActionsFile();
    createMenuFile();
    createWidget();
    createLeftSideLayout();
}

MainWindow::~MainWindow()
{

}


void MainWindow::createActionsFile()
{
    //File : Load Tiles
    m_actionLoadTiles = new QAction ("&Load Tiles",this);
    m_actionLoadTiles -> setShortcut(tr("Ctrl+L"));
    connect(m_actionLoadTiles, SIGNAL(triggered()), this, SLOT(s_loadTiles()));

    //File : Save
    m_actionSave = new QAction ("&Save", this);
    m_actionSave -> setShortcut(tr("Ctrl+S"));
    connect(m_actionSave, SIGNAL(triggered()),this, SLOT(s_save()));

    //File : Save-As
    m_actionSaveAs = new QAction ("&SaveAs", this);
    m_actionSaveAs -> setShortcut(tr("Ctrl+Shift+S"));
    connect(m_actionSaveAs, SIGNAL(triggered()),this, SLOT(s_saveAs()));

    //File : Quit
    m_actionQuit = new QAction ("&Quit", this);
    m_actionQuit -> setShortcut(tr("Ctrl+Q"));
    connect(m_actionQuit, SIGNAL(triggered()),this, SLOT(s_quit()));
}

//Associating the actions with File Menu
void MainWindow::createMenuFile()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actionLoadTiles);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actionSave);
    m_menuFile->addAction(m_actionSaveAs);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actionQuit);
}


void MainWindow::createWidget()
{
    //Making Horizontal Slider
    m_slider = new QSlider(Qt::Horizontal);
    m_slider ->setMinimum(1);
    m_slider->setMaximum(10);
    m_slider->setValue(1);

    //Making SpinBox
    m_spinBox = new QSpinBox;
    m_spinBox ->setMinimum(1);
    m_spinBox ->setMaximum(10);
    m_spinBox ->setValue(1);

    //Creating Push Buttons: Load & Play/Pause
    m_LoadButton = new QPushButton("Load");
    m_playPause  = new QPushButton("Play/Pause");
    m_reset      = new QPushButton("Reset");

    //Creating Checked Boxes
    m_scaleTiles  = new QCheckBox("Scale Tiles");
    m_rotateTiles = new QCheckBox("Rotate Tiles");
    m_showCent    = new QCheckBox("Show Centroid");
}


void MainWindow::createLeftSideLayout()
{
    //Placing the Components:

    //1. Adding the Pushbuttons to a HBox
    QHBoxLayout *HorzButtons = new QHBoxLayout;
    HorzButtons -> addWidget(m_LoadButton);
    HorzButtons -> addWidget(m_playPause);
    HorzButtons -> addWidget(m_reset);

    //2. Adding Slider/SpinBox widget to a hBox
    QLabel *speed = new QLabel("Speed");
    QHBoxLayout *HorzSlider = new QHBoxLayout;
    HorzSlider -> addWidget(speed);
    HorzSlider -> addWidget(m_slider);
    HorzSlider -> addWidget(m_spinBox);

    //3. Adding all of the widgets to a
    //main Vertical Layout on the left Side.
    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout -> addLayout(HorzButtons);
    leftVLayout -> addWidget(m_scaleTiles);
    leftVLayout -> addWidget(m_rotateTiles);
    leftVLayout -> addWidget(m_showCent);
    leftVLayout -> addLayout(HorzSlider);
    leftVLayout ->addStretch();

    // Create widget: set it to be app's central widget
    m_glwidget = new GLWidget;  // widget for OpenGL drawing

    //4.Adding all widgets and OpenGl Drawing to a Horz Lay
    QHBoxLayout *HorzLayout = new QHBoxLayout;
    HorzLayout -> addLayout(leftVLayout);
    HorzLayout -> addWidget(m_glwidget);
    HorzLayout -> setStretchFactor(leftVLayout, 1);
    HorzLayout -> setStretchFactor(m_glwidget,  4);

    //5.Setting Main Layout
    QWidget *window = new QWidget;
    window ->setLayout(HorzLayout);
    setCentralWidget(window);

    //Signal Slot Connections
    connect(m_LoadButton,SIGNAL(clicked()),this, SLOT(s_loadTiles()));
    connect(m_playPause, SIGNAL(clicked()), m_glwidget, SLOT(s_Play()));

    connect(m_showCent,SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setCentroid(int)));
    connect(m_scaleTiles, SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setScale(int)));
    connect(m_rotateTiles, SIGNAL(stateChanged(int)), m_glwidget, SLOT(s_setRotate(int)));

    connect(m_slider,  SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setSpeedMultiplier(int)));
    connect(m_slider,  SIGNAL(valueChanged(int)), m_spinBox, SLOT(setValue(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider,  SLOT(setValue(int)));

    //Reset Signal-Slot Connection
    connect(m_reset, SIGNAL(clicked()),m_glwidget, SLOT(s_reset()));
    connect(m_reset, SIGNAL(clicked()),this, SLOT(s_resetWidget()));

}

//Slot Functions:

void  MainWindow::s_loadTiles ()
{
    // launch file dialog and get file containing tile geometry
    QString fileName  = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName == NULL) return;

    // assign m_tiles to the OpenGL widget
    m_glwidget->loadTiles(fileName,0);

    // launch file dialog and get file containing tile geometry
    fileName  = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName == NULL) return;

    m_glwidget->loadTiles(fileName,1);

    m_glwidget->loadTexture();

    m_glwidget->setTimer();
}


void MainWindow::s_save       () {}
void MainWindow::s_saveAs     () {}


// Slot Function for Quiting Project
void MainWindow::s_quit()
{
   exit(0);
}


//Reset Slot Functions
void MainWindow::s_resetWidget()
{
    s_resetRotateChecked();
    s_resetScaleChecked();
    s_resetCentroid();
    s_resetSlider();
    s_resetSpinBox();
}

void MainWindow::s_resetRotateChecked()
{
    m_rotateTiles->setChecked(false);
}

void MainWindow::s_resetScaleChecked()
{
    m_scaleTiles->setChecked(false);
}

void  MainWindow::s_resetCentroid  ()
{
    m_showCent->setChecked(false);
}

void MainWindow::s_resetSlider()
{
    m_slider-> setValue(1);
}

void MainWindow::s_resetSpinBox()
{
    m_spinBox ->setValue(1);
}

