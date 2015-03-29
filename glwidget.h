

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
     GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void  setTiles(vector<Tile> &);
    void  setTimer();
    void  drawTiles();
    bool  mFlagCentroid;
    bool  mFlagRotate;
    bool  mFlagScale;

signals:

public slots:
    void s_Play();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    vector<Tile>	mTiles;

    bool            mPlay;

    QTimer          *mTimer;
};

#endif	// GLWIDGET_H
