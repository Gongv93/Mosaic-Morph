/* ========================================================================
   File:    glwidget.cpp
   Class:   Senior Design Csc 59866
   Creator: Asad Kamal, Vincent Gong, RuLong Haung
   ======================================================================== */

#include "glwidget.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Constructor
//

GLWidget::GLWidget()
{
    m_speedMulti      = 0.4f;   // Set angle multiplier
    m_scale           = 1.0;    // Set Scale factor
#if MOSAIC_VERSION
    m_r2              = 0.0f;   // Set Radius 1
    m_r22             = 0.0f;   // Set Radius 2
#else
    m_t = 0.0;
    m_state = 0;
#endif


    m_play            = false;  // Set play flag
    m_flagCentroid    = false;  // Set centroid flag
    m_flagRotate      = false;  // Set rotate flag
    m_flagScale       = false;  // Set scale flag
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Destructor
//

GLWidget::~GLWidget()
{}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// InitializeGL:
// 
// Initializes our GLWidget by enabling smooth point, set background,
// enable depth test, and enable lighting
//

void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // Enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // Enable depth test
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Set background color to black and opaque
    glClearDepth(1.0f);                                 // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);                              // Enable lighting
    glEnable(GL_LIGHT0);                                // Enable light #0
    glEnable(GL_LIGHT1);                                // Enable light #1
    glEnable(GL_NORMALIZE);                             // Automatically normalize normals
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setTimer:
//
// Initializes our timer for 1 frame per 10ms. Once 10ms past then
// we call updateGL to update our widget.
//

void GLWidget::setTimer()
{
    // Set Timer to 10 ms
    m_Timer = new QTimer(this);
    m_Timer->start(10);

    // Set conncetion for timer
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// resizeGL:
// 
// Sets the size of our GLWidget.
//

void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, .01, 1000.);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// paintGL:
//
// This updates our animation states such as tile angle and size.
// We also set light color, light position, and camera posistion.
// Also it call our render in this function to see our animation.
//

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
#if MOSAIC_VERSION
    gluLookAt(0.0, -2.0, 4.0, 0, 0, 0, 0.0, 1.0, 0.0);
#else
    gluLookAt(0.0, 0.0, 4.0, 0, 0, 0, 0.0, 1.0, 0.0);
#endif
    // Render our tiles
    drawTiles();

    // Update our tiles for next frame
    updateTiles();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// drawTiles:
// 
// This is our renderer where we draw and animates our tiles. It
// first renders the first set of tiles and then the second set.
//

void GLWidget::drawTiles()
{
#if MOSAIC_VERSION
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
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glBegin(GL_POLYGON);                                    // set polygon mode
        int n_vtx = m_tiles[i].num();                           // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {                          // visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);               // assign (x,y) coords to vtx
            glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);
            glVertex3f(vtx.x(), vtx.y(), m_tiles[i].depth());   // assign vtx as next polygon vertex
        }
        glEnd();                                                // end polygon mode

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
        glBindTexture(GL_TEXTURE_2D, m_texture2);

        glBegin(GL_POLYGON);                                    // set polygon mode
        int n_vtx = m_tiles2[i].num();                          // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {                          // visit each tile vertex
            QVector2D vtx = m_tiles2[i].vertex(j);              // assign (x,y) coords to vtx
            glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);
            glVertex3f(vtx.x(), vtx.y(), m_tiles2[i].depth());  // assign vtx as next polygon vertex
        }
        glEnd();                                                // end polygon mode

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
#else

    if(m_morphTile.num() == 0) return;
    //if(m_tiles.empty()) return;

    glColor3f(1.0,1.0,1.0);

    glBegin(GL_POLYGON);                                    // set polygon mode
    int n_vtx = m_morphTile.num();                           // get number of tile vertices
    for(int j = 0; j<n_vtx; ++j) {                          // visit each tile vertex
        QVector2D vtx = m_morphTile.vertex(j);
        glVertex3f(vtx.x(), vtx.y(), 0.0);   // assign vtx as next polygon vertex
    }
    glEnd();                                                // end polygon mode

#endif
}

void GLWidget::updateTiles()
{
#if MOSAIC_VERSION
    if(m_play) {
        // First scale down to .75
        if(m_scale > .75) {
            m_scale = m_scale - (0.005 * m_speedMulti) ;
        }

        // Then explode
        else {
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
#else

    switch(m_state) {
        // Increase
        case 0:
        {
            m_t += 0.005f * m_speedMulti;
            if(m_t >= 1.0) {
                // Make sure it doesnt go past 1 and change state
                m_t = 1.0;
                m_state = 1;
            }
        } break;

        // Decrease
        case 1:
        {
            m_t -= 0.005f * m_speedMulti;
            if(m_t <= 0.0) {
                // Make sure it doesnt go past 0 and change state
                m_t = 0.0;
                m_state = 0;
            }
        } break;
    }

    Tile tempTile;
    m_interTile.InterPolate(m_t, tempTile);
    m_morphTile = tempTile;

#endif

}

#if MOSAIC_VERSION
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// radialMotion:
//
// Updates the first set of tile's angle and depth based on our
// radius m_r2.
//

void GLWidget::radialMotion(Tile &tile)
{
    QVector2D cen = tile.centroid();
    float dist2 = (cen.x()*cen.x() + cen.y()*cen.y());
    if(dist2 <= m_r2) {
        tile.updateDepth((rand() % 10000) / 500000.0);
        tile.updateAngles((rand() % 10) / 3.0, (rand() % 10) / 3.0, (rand() % 10) / 10.0);
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// radialMotion2:
//
// Updates the second set of tile's angle and depth based on our
// radius m_r22.
//

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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// loadTexture:
//
// Loads our texture for our first set of tiles.
//

void GLWidget::loadTexture()
{
    QImage img(m_imgFileName);
    QImage GL_formatted_image = QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &m_texture);

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
   
    //texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //generate the texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(),
                  GL_formatted_image.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits() );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// loadTexture2:
//
// Loads our texture for our second set of tiles.
//

void GLWidget::loadTexture2()
{
    QImage img2(m_imgFileName2);
    QImage GL_formatted_image2 = QGLWidget::convertToGLFormat(img2);

    glGenTextures(2, &m_texture2);

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    
   
    //texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //generate the texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image2.width(),
                  GL_formatted_image2.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image2.bits() );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

#else

void GLWidget::getMorph()
{
    m_interTile.FindSourceDest(m_tiles[0], m_tiles2[0]);
    m_interTile.InterPolate(0, m_morphTile);
}

#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// loadTiles:
//
// Loads our tiles so we are able to render them later on. We also
// get the name of the our texture file for texture mapping.
//

void GLWidget::loadTiles(QString &fileName, int flag)
{

        QFileInfo *info = new QFileInfo(fileName);

        if(flag == 0)
            m_imgFileName = info->path() + "/" + info->baseName() + ".jpg";
        if(flag == 1)
            m_imgFileName2 = info->path() + "/" + info->baseName() + ".jpg";

        // open file for reading a stream of text
        QFile   file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);

        // read first three lines to get mosaic width, height, and number of tiles;
        // start with first line: mosaic width
        QString line = in.readLine();       // read mosaic width (inches)
        int   w  = line.toInt();            // convert string to int
        float w2 = w >> 1;                  // half-width for computing center

        // read second line: mosaic height
        line = in.readLine();               // read mosaic height (inches)
        int h  = line.toInt();              // convert string to int
        int h2 = h >> 1;                    // half-height for computing center

        // read third line: number of tiles
        line = in.readLine();               // read number of tiles
        int n_tiles = line.toInt();         // convert string to int
        for (int i = 0; i<n_tiles; ++i) {
            Tile tile;
            line = in.readLine();           // read number of vertices
            int n_vertices = line.toInt();  // convert string to int
            tile.setNum(n_vertices);        // set it in tile class

            // visit all vertices
            for (int j = 0; j<n_vertices; ++j) {
                line = in.readLine();       // read coordinates
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

//Slot Functions:

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_play:
//
// Stop or plays our animation by stopping or starting our timer.
//

void GLWidget::s_play()
{
    m_play =!m_play;
    if (m_play) m_Timer->start();
    else        m_Timer->stop();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setSpeedMultiplier:
//
// Sets our multiplier for our speed of animation.
//

void GLWidget::s_setSpeedMultiplier(int spinnerVal)
{
    m_speedMulti = spinnerVal;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setcentroid:
//
// Sets our centroid flag to decide when to show our centroid
//

void GLWidget::s_setCentroid(int flag)
{
    m_flagCentroid = flag;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setScale
//
// Sets our scaling flag to decide when to scale our tiles.
//

void GLWidget::s_setScale(int flag)
{
    m_flagScale = flag;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setRotate:
//
// Sets our rotating flag to decide when to rotate our tiles.
//

void GLWidget::s_setRotate(int flag)
{
    m_flagRotate = flag;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_reset:
//
// Resets our animation to its inital state
//

void GLWidget::s_reset()
{
#if MOSAIC_VERSION
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
#endif
}

