
#ifndef TILE_H
#define TILE_H


#include <vector>
#include <QVector2D>
#include <QColor>


using namespace std;

class Tile {
public:
    Tile();
    void		setNum(int num);
    int		    num();
    void		addVertex(QVector2D &);
    void		setRandColor();
    void        setCentroid();
    QColor		color();
    QVector2D	vertex(int);
    QVector2D   centroid();

private:
    int			       m_num;		 //number of vertices
    vector<QVector2D>  m_vertices;	//coordinates
    QVector2D		   m_centroid; //tile centroid
    QColor			   m_color;	  //tile color
};

#endif	// TILE_H
