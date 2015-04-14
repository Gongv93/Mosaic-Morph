

// ======================================================================
//
// Tile.cpp - Tile class
//
// ======================================================================

#include "Tile.h"

Tile::Tile()
    : m_num(0) , m_depth(0.0f), m_angleX(0.0f), m_angleY(0.0f), m_angleZ(0.0f)
{}



void Tile::setNum(int num)
{
    m_num = num;
}



int Tile::num()
{
    return m_num;
}



void Tile::addVertex(QVector2D &vtx)
{
    m_vertices.push_back(vtx);
}



QVector2D Tile::vertex(int k)
{
    return m_vertices[k];
}



void Tile::setRandColor()
{
    m_color.setRgb((rand() % 256), (rand() % 256), (rand() % 256));
}



QColor Tile::color()
{
    return m_color;
}

void Tile::setCentroid()
{
    float cx = 0.;
    float cy = 0.;
    for (int i = 0; i<m_num; ++i) {
        QVector2D vtx = m_vertices[i];
        cx += vtx.x();
        cy += vtx.y();
    }
    m_centroid.setX(cx / m_num);
    m_centroid.setY(cy / m_num);
}


QVector2D Tile::centroid()
{
    return m_centroid;
}

float   Tile::depth()
{
    return m_depth;
}


void    Tile::setDepth(float depth)
{
    m_depth = depth;
}

void    Tile::updateDepth(float depth)
{
    m_depth += depth;
}


float   Tile::angleX()
{
    return m_angleX;
}

float   Tile::angleY()
{
    return m_angleY;
}

float   Tile::angleZ()
{
    return m_angleZ;
}

void    Tile::setAngles(float x, float y, float z)
{
    m_angleX = x;
    m_angleY = y;
    m_angleZ = z;
}

void    Tile::updateAngles(float x, float y, float z)
{
    m_angleX += x;
    m_angleY += y;
    m_angleZ += z;
}
