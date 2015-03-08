
#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_tiles.clear();
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



void GLWidget::setTiles(vector<Tile> &tiles)
{
    m_tiles = tiles;
    updateGL(); //or setTimer
}



void GLWidget::drawTiles()
{
    // error checking
    if (m_tiles.empty()) return;

    // for each tile, get color and draw polygon
    int n_tiles = m_tiles.size();
    for (int i = 0; i<n_tiles; ++i) {

        // draw centroid
        if(clicked) {
            glColor3f(1.0f, 1.0f, 1.0f);                    // Set color for point
            glPointSize(5.0f);                              // Set point size
            glBegin(GL_POINTS);                             // set point mode
            QVector2D centroid = m_tiles[i].centroid();     // Get centroid
            glVertex3f(centroid.x(), centroid.y(), 0.0f);   // assign (x,y) coords
            glEnd();     
        } 

        // get tile color and pass it to OpenGL
        QColor color = m_tiles[i].color();
        glColor3f(color.redF(), color.greenF(), color.blueF());
        
        // draw tile polygon
        glBegin(GL_POLYGON);			             // set polygon mode
        int n_vtx = m_tiles[i].num();		         // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {		         // visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);	 // assign (x,y) coords to vtx
            glVertex3f(vtx.x(), vtx.y(), 0.0f);	     // assign vtx as next polygon vertex
        }
        glEnd();				                     // end polygon mode
    }
}
