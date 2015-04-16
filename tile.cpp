/* ========================================================================
   File:    tile.cpp
   Class:   Senior Design Csc 59866
   Creator: Asad Kamal, Vincent Gong, RuLong Haung
   ======================================================================== */

#include "Tile.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Constructor
//

Tile::Tile()
    : m_num(0) , m_depth(0.0f), m_angleX(0.0f), m_angleY(0.0f), m_angleZ(0.0f)
{}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Destructor
//

void Tile::setNum(int num)
{
    m_num = num;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// num:
//
// Returns the nummber of vertices.
//

int Tile::num()
{
    return m_num;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// addVertex:
//
// Adds a vertex to the tile by pushing back in m_vertices.
//

void Tile::addVertex(QVector2D &vtx)
{
    m_vertices.push_back(vtx);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// vertex:
//
// Returns the vertex k in the tile.
//

QVector2D Tile::vertex(int k)
{
    return m_vertices[k];
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setRandColor:
//
// Sets a random color for the tile.
//

void Tile::setRandColor()
{
    m_color.setRgb((rand() % 256), (rand() % 256), (rand() % 256));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// color:
//
// Returns the color of the tile.
//

QColor Tile::color()
{
    return m_color;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setCentroid:
//
// Calculates the value of the centroid and sets it to m_centroid.
//

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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// centroid:
//
// Returns the tiles centroid.
//

QVector2D Tile::centroid()
{
    return m_centroid;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// depth:
//
// Returns the current depth of the tile.
//

float   Tile::depth()
{
    return m_depth;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setDepth:
//
// Set the depth of the tile.
//

void    Tile::setDepth(float depth)
{
    m_depth = depth;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// updateDepth:
//
// Updates the depth of the tile by adding the input to the current
// depth.
//

void    Tile::updateDepth(float depth)
{
    m_depth += depth;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// angleX:
//
// Returns the X angle rotation of the tile.
//

float   Tile::angleX()
{
    return m_angleX;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// angleY:
//
// Returns the Y angle rotation of the tile.
//

float   Tile::angleY()
{
    return m_angleY;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// angleZ:
//
// Returns the Z angle rotation of the tile.
//

float   Tile::angleZ()
{
    return m_angleZ;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setAngles:
//
// Set the rotation angles of the tile for x, y and z.
//

void    Tile::setAngles(float x, float y, float z) 
{
    m_angleX = x;
    m_angleY = y;
    m_angleZ = z;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// updateAngles:
//
// Update the rotation angle of the tile for x, y, and z by adding
// the respected inputs.
//

void    Tile::updateAngles(float x, float y, float z)
{
    m_angleX += x;
    m_angleY += y;
    m_angleZ += z;
}
