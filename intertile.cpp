/* ========================================================================
   File:    InterTile.cpp
   Class:   Senior Design Csc 59866
   Creator: Asad Kamal, Vincent Gong, RuLong Haung
   ======================================================================== */


#include "intertile.h"

bool comparator(const MyPair l, const MyPair r) {return (l.first < r.first);}

InterTile::InterTile(){}
InterTile::~InterTile(){};

void InterTile::FindSourceDest(Tile tile1, Tile tile2)
{
    vector<QVector2D> source;       // Tile1
    vector<QVector2D> destination;  // Tile2

    //get number of tile vertices
    int n1,n2;
    n1 = tile1.num();
    n2 = tile2.num();

    // Get centroid
    QVector2D cen1 = tile1.centroid();
    QVector2D cen2 = tile2.centroid();
    QVector2D t = cen2 - cen1;

    // Fix tile2 to have same centroid as tile1
    for(int i = 0; i<n2 ; ++i) {
        QVector2D vtx = tile2.vertex(i);
        vtx = vtx - t;
        tile2.setVertex(i,vtx);
    }

    // Find the new centroid
    tile2.setCentroid();

    // Get intersections through destination tile
    for(int i = 0; i < n1 ; ++i) {
        // Get first vtx and draw a line from that and the centroid
        const QVector2D vtx = tile1.vertex(i);
        float len = (vtx - cen1).length()*10;   // Increase the length by 10 times
        QLineF line1(cen1.x(),cen1.y(),vtx.x(),vtx.y());    // Create Line
        line1.setLength(len);
        int k = n2-1;   // Get last point in tile 2

        // Find which pair of vtx it intersects by going through all of them
        for (int j = 0; j < n2; ++j) {
            QVector2D vtx1 = tile2.vertex(k);
            QVector2D vtx2 = tile2.vertex(j);

            // Create the line from two of destination vtx
            QLineF line2(vtx1.x(),vtx1.y(),vtx2.x(),vtx2.y());
            QPointF IntersecPoint;
            QLineF::IntersectType intersectType = line1.intersect(line2,&IntersecPoint);

            // If theres an intersection push the points
            if(intersectType == QLineF::BoundedIntersection) {
                source.push_back(vtx);
                QVector2D dest(IntersecPoint.x(),IntersecPoint.y());
                destination.push_back(dest);
                break;
            }
            k = j;
        }
    }

    // Get intersections through source
    for(int i = 0; i<n2; ++i) {
        // Get first vtx and draw a line from that to the centroid
        const QVector2D vtx = tile2.vertex(i);
        float len = (vtx - cen2).length()*10;
        QLineF line1(cen2.x(),cen2.y(),vtx.x(),vtx.y());
        line1.setLength(len);
        int k = n1-1; //last point

        // Find which pair of vtx it intersects
        for (int j = 0; j < n1; ++j) {
            QVector2D vtx1 = tile1.vertex(k);
            QVector2D vtx2 = tile1.vertex(j);

            // Create Line
            QLineF line2(vtx1.x(),vtx1.y(),vtx2.x(),vtx2.y());
            QPointF IntersecPoint;
            QLineF::IntersectType intersectType = line1.intersect(line2,&IntersecPoint);

            // Intersection check
            if(intersectType == QLineF::BoundedIntersection) {
                destination.push_back(vtx);
                QVector2D dest(IntersecPoint.x(),IntersecPoint.y());
                source.push_back(dest);
                break;
            }
            k = j;
        }
    }

    // Set our first index as angle 0 so we can use it to compare the rest
    vector<MyPair> pairs;
    QVector2D &vtx = source[0];
    QVector2D v1   = vtx - cen1;    // Change to respect to origin instead of centroid
    MyPair pair(0.0f,0);
    pairs.push_back(pair);

    for (unsigned int i = 1; i < source.size(); ++i) {
        QVector2D &vtx = source[i];
        QVector2D v2   = vtx-cen1;
        float x1 = v1.x();
        float y1 = v1.y();
        float x2 = v2.x();
        float y2 = v2.y();

        // Find the angle at a certain index respect to point 1
        float dot = x1*x2 + y1*y2;
        float det = x1*x2 - y1*y2;
        float angle = atan2(det, dot);

        MyPair pair(angle,i);
        pairs.push_back(pair);
    }

    std::sort(pairs.begin(),pairs.end(),comparator);

    for(uint i =0; i < pairs.size();++i) {
        m_source.push_back(source[pairs[i].second]);
        m_destination.push_back(destination[pairs[i].second]); //not sure
    }
}

void InterTile::InterPolate(float t, Tile &tile)
{
    tile.setNum(m_source.size());
    for(uint i=0; i < m_source.size(); ++i) {
        QVector2D &src = m_source[i];
        QVector2D &des = m_destination[i];
        QVector2D inter(src*(1-t)+des*t);
        tile.addVertex(inter);
    }
}
