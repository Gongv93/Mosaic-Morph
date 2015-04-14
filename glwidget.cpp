

#include "glwidget.h"

GLWidget::GLWidget()
{
    m_tiles.clear();
    m_angle           = 0.0;     // Init angle
    m_baseAngleSpeed  = 0.001f;  // Set base angle rotation speed
    m_angleMulti      = 1.0f;    // Set angle multiplier

    m_scale           = 1.0;
    m_baseScaleFreq   = 0.003f;  // Set base scale speed
    m_scaleAngle      = 0.0f;

    m_play            = true;
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
    if (m_tiles.empty()) return;

    // for each tile, get color and draw polygon
    int n_tiles = m_tiles.size();
    for (int i = 0; i<n_tiles; ++i) {
        QVector2D centroid = m_tiles[i].centroid();

        // draw centroid
        if(m_flagCentroid)
        {
            glColor3f(1.0f, 1.0f, 1.0f);                    // Set color for point
            glPointSize(4.0f);                              // Set point size
            glBegin(GL_POINTS);                             // set point mode
            glVertex3f(centroid.x(), centroid.y(), 0.0f);   // assign (x,y) coords
            glEnd();
        }

        // get tile color and pass it to OpenGL
        QColor color = m_tiles[i].color();
        glColor3f(color.redF(), color.greenF(), color.blueF());

        // Update tile transfomation params
        if(m_flagRotate) {
            m_angle += m_baseAngleSpeed * m_angleMulti;
            if(m_angle >= 360.0)  m_angle = 0.0f;
        }

        // Scaling is done by a sine curve
        if(m_flagScale) {
            m_scale = 1.0 + (0.9 * qSin(m_scaleAngle * m_baseScaleFreq * m_angleMulti));
            m_scaleAngle += 0.01f;
            if(m_angle == 3.14) m_scaleAngle = 0.0;
        }

        // Apply transformation
        glPushMatrix();

        glTranslatef(centroid.x(), centroid.y(), 0);
        glRotatef(m_angle, 0.0, 0.0, 1.0);
        glScalef(m_scale, m_scale,m_scale);
        glTranslatef(-centroid.x(), -centroid.y(), 0);

        glBegin(GL_POLYGON);                         // set polygon mode
        int n_vtx = m_tiles[i].num();                // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {               // visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);    // assign (x,y) coords to vtx
            glVertex3f(vtx.x(), vtx.y(), 0.0f);      // assign vtx as next polygon vertex
        }
        glEnd();                                     // end polygon mode


        glPopMatrix();
    }

}


void GLWidget::loadTiles(QString &fileName)
{

        QFileInfo *info = new QFileInfo(fileName);
        m_imgFileName = info->path() + "/" + info->baseName() + ".jpg";

        // open file for reading a stream of text
        QFile	file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);

        // read first three lines to get mosaic width, height, and number of tiles;
        // start with first line: mosaic width
        QString line = in.readLine();		// read mosaic width (inches)
        int   w  = line.toInt();		// convert string to int
        float w2 = w >> 1;			// half-width for computing center

        // read second line: mosaic height
        line = in.readLine();			// read mosaic height (inches)
        int h  = line.toInt();			// convert string to int
        int h2 = h >> 1;			// half-height for computing center

        // read third line: number of tiles
        line = in.readLine();			// read number of tiles
        int n_tiles = line.toInt();		// convert string to int
        for (int i = 0; i<n_tiles; ++i)
        {
            Tile tile;
            line = in.readLine();		// read number of vertices
            int n_vertices = line.toInt();	// convert string to int
            tile.setNum(n_vertices);	// set it in tile class

            // visit all vertices
            for (int j = 0; j<n_vertices; ++j)
            {
                line = in.readLine();	// read coordinates
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
            m_tiles.push_back(tile);
        }

        // close file
        file.close();
}


//Slot Functions:
void GLWidget::s_Play()
{
    m_play =!m_play;
    if (m_play) m_Timer->start();
    else        m_Timer->stop();

}

void GLWidget::s_setAngleMultiplier(int spinnerVal)
{
    if(spinnerVal > 5)      m_angleMulti = (spinnerVal - 5) * 2;
    else if(spinnerVal < 5) m_angleMulti = qPow(.5, 5 - spinnerVal);
    else                    m_angleMulti = 1.0f;

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
    s_resetRotate();
    s_resetScale();
}


void GLWidget::s_resetRotate()
{
    m_angle=0.0;

}


void GLWidget::s_resetScale()
{
    m_scale=1.0;
}

