
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <QtCore/qmath.h>
#include <GL/glu.h>
#include <vector>
#include "Tile.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
         GLWidget();
        ~GLWidget();

        void        setTiles        (vector<Tile> &);
        void        loadTiles	    (QString &, int);
        void        drawTiles       ();
        void        setTimer        ();
        void        radialMotion    (Tile &);
        void        radialMotion2   (Tile &);
        void        loadTexture     ();
<<<<<<< HEAD
=======
        void        loadTexture2    ();
>>>>>>> e9619796b7b9adbc1a6b0010eb656c406ad85bbf
        QVector3D   computeNormal   (QVector2D &, QVector2D &, float, float);

    public slots:
        void s_play                 ();
        void s_setSpeedMultiplier   (int spinnerVal);
        void s_setCentroid          (int flag);
        void s_setScale             (int flag);
        void s_setRotate            (int flag);

        void    s_reset             ();



    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

    private:
        vector<Tile>    m_tiles;
        vector<Tile>    m_tiles2;
        QTimer*         m_Timer;
        bool            m_play;
        bool            m_flagCentroid;
        bool            m_flagRotate;
        bool            m_flagScale;

        GLuint          m_texture;
        QString         m_imgFileName;
        
        GLuint          m_texture2;
        QString         m_imgFileName;
        QString         m_imgFileName2;


        float           m_r2;
        float           m_r22;
        float           m_speedMulti;
        float           m_scale;
};

#endif  // GLWIDGET_H
