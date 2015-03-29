
#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    mTiles.clear();
    mFlagCentroid = false;
    setTimer();
}



GLWidget::~GLWidget()
{}


void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawTiles();
}



void GLWidget::resizeGL(int w, int h)
{
    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0, -3, 3);
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

}


void GLWidget::setTimer() 
{
    // Set Timer to 10 ms
    mTimer = new QTimer(this);
    mTimer->start(10);

    // Set conncetion for timer
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
}


void GLWidget::setTiles(vector<Tile> &tiles)
{
    mTiles = tiles;
    //setTimer();
    //updateGL(); //or setTimer
}

 

void GLWidget::drawTiles()
{
    // error checking
    if (mTiles.empty()) return;

    // for each tile, get color and draw polygon
    int n_tiles = mTiles.size();
    for (int i = 0; i<n_tiles; ++i) {

        // draw centroid
        if(mFlagCentroid) {
            glColor3f(1.0f, 1.0f, 1.0f);                    // Set color for point
            glPointSize(4.0f);                              // Set point size
            glBegin(GL_POINTS);                             // set point mode
            QVector2D centroid = mTiles[i].centroid();     // Get centroid
            glVertex3f(centroid.x(), centroid.y(), 0.0f);   // assign (x,y) coords
            glEnd();
        }

        // get tile color and pass it to OpenGL
        QColor color = mTiles[i].color();
        glColor3f(color.redF(), color.greenF(), color.blueF());

        // draw tile polygon
        glBegin(GL_POLYGON);			             // set polygon mode
        int n_vtx = mTiles[i].num();		         // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {		         // visit each tile vertex
            QVector2D vtx = mTiles[i].vertex(j);	 // assign (x,y) coords to vtx
            glVertex3f(vtx.x(), vtx.y(), 0.0f);	     // assign vtx as next polygon vertex
        }
        glEnd();				                     // end polygon mode
    }
}


//***** Slots ******//
/*
void GLWidget::s_SetCentroid(bool Flag)
{
    mFlagCentroid = Flag;
}

void GLWidget::s_SetRotate(bool Flag)
{
    mFlagRotate = Flag;
}

void GLWidget::s_SetScale(bool Flag)
{
    mFlagScale = Flag;
}
*/
void GLWidget::s_Play()
{
    mPlay = !mPlay;

    if(mPlay) mTimer->start();
    else      mTimer->stop();
}