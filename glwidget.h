
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

    void  isClicked();
    void  setTiles(vector<Tile> &);
    void  drawTiles();

    bool clicked = false;

signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    vector<Tile>	m_tiles;
};

#endif	// GLWIDGET_H
