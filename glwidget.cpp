

#include "glwidget.h"
#include <cmath>

GLWidget::GLWidget()
{
    m_tiles.clear();

    m_speedMulti      = 0.4f;    // Set angle multiplier
    m_r2              = 0.0f;
    m_r22             = 0.0f;

    m_scale           = 1.0;

    m_play            = false;
    m_flagCentroid    = false;
    m_flagRotate      = false;
    m_flagScale       = false;
}

GLWidget::~GLWidget()
{}


void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // enable depth test
    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Set background color to black and opaque
    glClearDepth(1.0f);                                 // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);                              //Enable lighting
    glEnable(GL_LIGHT0);                                //Enable light #0
    glEnable(GL_LIGHT1);                                //Enable light #1
    glEnable(GL_NORMALIZE);                             //Automatically normalize normals

}


void GLWidget::paintGL()
{
        // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // add ambient light
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    // add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0  [] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos1  [] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    // update modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move camera and look at origin
    glTranslatef(0, 0, 3.0);
    gluLookAt(0.0, -2.0, 4.0, 0, 0, 0, 0.0, 1.0, 0.0);

    drawTiles();

    if(m_play) {
        // First scale down to .75
        if(m_scale > .75)
        {
            m_scale = m_scale - (0.005 * m_speedMulti) ;
        }

        // Then explode
       else
        {
            if(m_r2 < 5) {
                m_r2 += .003 * m_speedMulti;
                // Update each tile for the next render cycle
                int n_tiles = m_tiles.size();
                for(int i = 0; i < n_tiles; ++i) {
                    radialMotion(m_tiles[i]);
                }
            }
               
            // When the first radius is greater than 2 second tile will fall
            if(m_r2 > 0.05 &&  m_r22 < 5) {
                m_r22 += .003 * m_speedMulti;
                int n_tiles = m_tiles2.size();
                for(int i = 0; i < n_tiles; ++i) {
                    radialMotion2(m_tiles2[i]);
                }
            }
        }

    }


}


void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, .01, 1000.);

}


void GLWidget::setTimer()
{
    // Set Timer to 10 ms
    m_Timer = new QTimer(this);
    m_Timer->start(10);

    // Set conncetion for timer
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

void GLWidget::setTiles(vector<Tile> &tiles)
{
    m_tiles = tiles;
    setTimer(); //or updateGL
}



void GLWidget::drawTiles()
{
    // error checking
    if (m_tiles.empty() || m_tiles2.empty()) return;

    // Render first set of tiles
    int n_tiles = m_tiles.size();
    for (int i = 0; i<n_tiles; ++i) {
        QVector2D centroid = m_tiles[i].centroid();

        // draw centroid
        if(m_flagCentroid)
        {
            glColor3f(1.0f, 1.0f, 1.0f);                    // Set white color for point
            glPointSize(4.0f);                              // Set point size
            glBegin(GL_POINTS);                             // set point mode
            glVertex3f(centroid.x(), centroid.y(), 0.0f);   // assign (x,y) coords
            glEnd();
        }

        // get tile color and pass it to OpenGL
        QColor color = m_tiles[i].color();
        glColor3f(color.redF(), color.greenF(), color.blueF());

        // Apply transformation
        glPushMatrix();

        glTranslatef(centroid.x(), centroid.y(), m_tiles[i].depth());
        glScalef(m_scale, m_scale,m_scale);
        glRotatef(m_tiles[i].angleX(), 1.0, 0.0, 0.0);
        glRotatef(m_tiles[i].angleY(), 0.0, 1.0, 0.0);
        glRotatef(m_tiles[i].angleZ(), 0.0, 0.0, 1.0);
        glTranslatef(-centroid.x(), -centroid.y(), -m_tiles[i].depth());

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE, m_texture);

        glBegin(GL_POLYGON);                         // set polygon mode
        int n_vtx = m_tiles[i].num();                // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {               // visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);    // assign (x,y) coords to vtx
            glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);
            glVertex3f(vtx.x(), vtx.y(), m_tiles[i].depth());      // assign vtx as next polygon vertex
        }
        glEnd();                                     // end polygon mode

        for(int j=0; j<n_vtx-1;j++){
               QVector2D vtx1 = m_tiles[i].vertex(j);
               QVector2D vtx2 = m_tiles[i].vertex(j+1);

               glBegin(GL_POLYGON);
                   glVertex3f(vtx1.x(),vtx1.y(),m_tiles[i].depth());
                   glVertex3f(vtx2.x(),vtx2.y(),m_tiles[i].depth());
                   glVertex3f(vtx2.x(),vtx2.y(),m_tiles[i].depth()-.02);
                   glVertex3f(vtx1.x(),vtx1.y(),m_tiles[i].depth()-.02);
                glEnd();
       }

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    
    // Second set of tiles
    n_tiles = m_tiles2.size();
    for(int i = 0; i < n_tiles; ++i) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE, m_texture);

        glBegin(GL_POLYGON);                         // set polygon mode
        int n_vtx = m_tiles2[i].num();                // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {               // visit each tile vertex
            QVector2D vtx = m_tiles2[i].vertex(j);    // assign (x,y) coords to vtx
            glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);
            glVertex3f(vtx.x(), vtx.y(), m_tiles2[i].depth());      // assign vtx as next polygon vertex
        }
        glEnd();                                     // end polygon mode

        for(int j=0; j<n_vtx-1;j++){
               QVector2D vtx1 = m_tiles2[i].vertex(j);
               QVector2D vtx2 = m_tiles2[i].vertex(j+1);

               glBegin(GL_POLYGON);
                   glVertex3f(vtx1.x(),vtx1.y(),m_tiles2[i].depth());
                   glVertex3f(vtx2.x(),vtx2.y(),m_tiles2[i].depth());
                   glVertex3f(vtx2.x(),vtx2.y(),m_tiles2[i].depth()-.02);
                   glVertex3f(vtx1.x(),vtx1.y(),m_tiles2[i].depth()-.02);
                glEnd();
       }

        glDisable(GL_TEXTURE_2D);
    }
    

}


void GLWidget::loadTiles(QString &fileName, int flag)
{

        QFileInfo *info = new QFileInfo(fileName);
        if(flag == 0) 
            m_imgFileName = info->path() + "/" + info->baseName() + ".jpg";

        // open file for reading a stream of text
        QFile   file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);

        // read first three lines to get mosaic width, height, and number of tiles;
        // start with first line: mosaic width
        QString line = in.readLine();       // read mosaic width (inches)
        int   w  = line.toInt();        // convert string to int
        float w2 = w >> 1;          // half-width for computing center

        // read second line: mosaic height
        line = in.readLine();           // read mosaic height (inches)
        int h  = line.toInt();          // convert string to int
        int h2 = h >> 1;            // half-height for computing center

        // read third line: number of tiles
        line = in.readLine();           // read number of tiles
        int n_tiles = line.toInt();     // convert string to int
        for (int i = 0; i<n_tiles; ++i)
        {
            Tile tile;
            line = in.readLine();       // read number of vertices
            int n_vertices = line.toInt();  // convert string to int
            tile.setNum(n_vertices);    // set it in tile class

            // visit all vertices
            for (int j = 0; j<n_vertices; ++j)
            {
                line = in.readLine();   // read coordinates
                double x = line.section(',', 0, 0).toDouble();
                double y = line.section(',', 1, 1).toDouble();

                // constructor function assigns normalized coordinate [-1,1] to vtx;
                // note: w2 and h2 are floats so that result is float too
                QVector2D vtx((x-w2)/w, (y-h2)/h);
                // add vertex to list of vertices in tile object
                tile.addVertex(vtx);
            }
            // set a random color for tile
            tile.setRandColor();
            tile.setCentroid();

            // add tile to the m_tiles array
            if(flag == 0) {
                tile.setDepth(0);
                m_tiles.push_back(tile);
            }
            if(flag == 1) {
                tile.setDepth(2.5);
                m_tiles2.push_back(tile);
            }
        }

        // close file
        file.close();

        //Play Button Set to Enabled Position
        m_play = true;
}


QVector3D GLWidget::computeNormal(QVector2D &vtx1, QVector2D &vtx2, float depth, float thickness)
{
    QVector3D p1(vtx1.x(), vtx1.y(), depth);
    QVector3D p2(vtx2.x(), vtx2.y(), depth);
    QVector3D p3(vtx1.x(), vtx1.y(), depth+thickness);
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;
    QVector3D v3 = QVector3D::crossProduct(v1, v2);
    return v3.normalized();
}


void GLWidget::radialMotion(Tile &tile)
{
    QVector2D cen = tile.centroid();
    float dist2 = (cen.x()*cen.x() + cen.y()*cen.y());
    if(dist2 <= m_r2) {
        tile.updateDepth((rand() % 10000) / 500000.0);
        tile.updateAngles((rand() % 10) / 3.0, (rand() % 10) / 3.0, (rand() % 10) / 10.0);
    }
}

void GLWidget::radialMotion2(Tile &tile)
{
    QVector2D cen = tile.centroid();
    float dist2 = (cen.x()*cen.x() + cen.y()*cen.y());
    if(dist2 <= m_r22) {
        if(tile.depth() > 0) {
            tile.updateDepth(-((rand() % 10000) / 500000.0));
            if(tile.depth() < 0) tile.setDepth(0);
        }
    }
}

void    GLWidget::loadTexture()
{
    QImage img(m_imgFileName);
    QImage GL_formatted_image = QGLWidget::convertToGLFormat(img);
    glGenTextures(2, &m_texture);

    //bind the texture ID
    //glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    
    /*
    else if(flag == 1) {
        QImage img(m_imgFileName2);
        GL_formatted_image = QGLWidget::convertToGLFormat(img);

        glGenTextures(1, &m_texture2);

        //bind the texture ID
        glBindTexture(GL_TEXTURE_2D, m_texture2);
    }
    */
    
    //texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //generate the texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(),
              GL_formatted_image.height(),
              0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits() );
    glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
}



//Slot Functions:
void GLWidget::s_play()
{
    m_play =!m_play;
    if (m_play) m_Timer->start();
    else        m_Timer->stop();

}

void GLWidget::s_setSpeedMultiplier(int spinnerVal)
{
    m_speedMulti = spinnerVal;
}

void GLWidget::s_setCentroid(int flag)
{
    m_flagCentroid = flag;
}

void GLWidget::s_setScale(int flag)
{
    m_flagScale = flag;
}

void GLWidget::s_setRotate(int flag)
{
    m_flagRotate = flag;
}


//Reset Slot Functions
void GLWidget::s_reset()
{
    int n_tiles = m_tiles.size();
    for(int i = 0; i < n_tiles; ++i) {
        m_tiles[i].setDepth(0);
        m_tiles[i].setAngles(0.0, 0.0, 0.0);
    }

    n_tiles = m_tiles2.size();
    for(int i = 0; i < n_tiles; ++i) {
        m_tiles2[i].setDepth(2.5);
        m_tiles2[i].setAngles(0.0, 0.0, 0.0);
    }

    m_scale = 1.0;
    m_r2    = 0.0;
    m_r22   = 0.0;
}

