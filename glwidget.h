


#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <QtCore/qmath.h>
#include <vector>
#include "Tile.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
         GLWidget();
        ~GLWidget();

        void  setTiles(vector<Tile> &);
        void  drawTiles();
        void  setTimer();

    public slots:
        void s_Play();
        void s_setAngleMultiplier(int spinnerVal);
        void s_setCentroid(int flag);
        void s_setScale(int flag);
        void s_setRotate(int flag);

        void    s_reset       ();
        void    s_resetRotate ();
        void    s_resetScale  ();


    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

    private:
        vector<Tile>    m_tiles;
        QTimer*         m_Timer;
        bool            m_play;
        bool            m_flagCentroid;
        bool            m_flagRotate;
        bool            m_flagScale;

        float           m_angle;
        float           m_baseAngleSpeed;
        float           m_angleMulti;

        float           m_scale;
        float           m_scaleAngle;
        float           m_baseScaleFreq;
};

#endif  // GLWIDGET_H
