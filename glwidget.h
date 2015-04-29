/* ========================================================================
   File:    glwidget.h
   Class:   Senior Design Csc 59866
   Creator: Asad Kamal, Vincent Gong, RuLong Haung
   ======================================================================== */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <QtCore/qmath.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include "Tile.h"
#if !MOSAIC_VERSION
#include "InterTile.h"
#endif

class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
         GLWidget();
        ~GLWidget();

        void loadTiles	     (QString &, int);
        void drawTiles       ();
        void setTimer        ();
        void updateTiles     ();

        void radialMotion    (Tile &);
        void radialMotion2   (Tile &);
        void loadTexture     ();
        void loadTexture2    ();

        void getMorph        ();


    public slots:
        void s_play                 ();
        void s_setSpeedMultiplier   (int);
        void s_setCentroid          (int);
        void s_setScale             (int);
        void s_setRotate            (int);
        void s_reset                ();

    protected:
        void initializeGL   ();
        void paintGL        ();
        void resizeGL       (int, int);

    private:
        vector<Tile>    m_tiles;
        vector<Tile>    m_tiles2;
        QTimer*         m_Timer;
        bool            m_play;
        bool            m_flagCentroid;
        bool            m_flagRotate;
        bool            m_flagScale;

        QString         m_imgFileName;
        QString         m_imgFileName2;

        float           m_speedMulti;
        float           m_scale;

#if MOSAIC_VERSION
        GLuint          m_texture;
        GLuint          m_texture2;
        float           m_r2;
        float           m_r22;
#else
        int             m_state;
        Tile            m_morphTile;
        InterTile       m_interTile;
        float           m_t;
#endif

};

#endif  // GLWIDGET_H
