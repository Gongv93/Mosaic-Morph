#ifndef INTERTILE_H
#define INTERTILE_H

#include <QVector2D>
#include <QLineF>
#include <vector>
#include <cmath>
#include <utility>
#include "Tile.h"

typedef std:: pair<float,int> MyPair;

class InterTile
{
public:
    InterTile();
    ~InterTile();

    void FindSourceDest      (Tile ,Tile);
    void InterPolate         (float, Tile &);

private:
    vector<QVector2D> m_source;
    vector<QVector2D> m_destination;

};

#endif
