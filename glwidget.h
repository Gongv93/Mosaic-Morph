
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
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
    bool  m_FlagCentroid;
    bool  m_FlagRotate;
    bool  m_FlagScale;


public slots:
    void s_Play();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    vector<Tile>    m_tiles;
    bool            m_play;
    QTimer*         m_Timer;
    float           m_angle;
    float           m_scale;




};

#endif  // GLWIDGET_H
