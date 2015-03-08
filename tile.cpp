// ======================================================================
//
// Tile.cpp - Tile class
//
// ======================================================================

#include "Tile.h"

Tile::Tile()
    : m_num(0)
{}



void Tile::setNum(int num)
{
    m_num = num;
}


void Tile::setRandColor()
{
    m_color.setRgb((rand() % 256), (rand() % 256), (rand() % 256));
}

void Tile::setCentroid()
{
    int i; 
    float x = 0.0, y = 0.0;

    // Summing up the x and y coords
    for(i = 0; i < m_num; i++) {
        x += m_vertices[i].x();
        y += m_vertices[i].y();
    }

    // Setting the centroids 
    m_centroid.setX(x/m_num);
    m_centroid.setY(y/m_num);
}


QVector2D Tile::centroid(){
    return m_centroid;
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



QColor Tile::color()
{
    return m_color;
}

