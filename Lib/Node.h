#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QMouseEvent>

#include <Edge.h>
//class Node
//class Edge;
#include "MyQGraphicsView.h"
//#include <QGraphWidget> 

class Node : public QObject, public QGraphicsItem
{
    Q_OBJECT
    static int inc_id;
    int id;
    int value;
    int size;
    int half_size;
    int pathValue;
    public:
       Node( MyQGraphicsView* pointer,int val=10,int a_size=50);
       virtual ~Node();
       int getId(){return id;}
       static int getLastId(){ return inc_id; }
       static void resetId(){ inc_id = 0; }
       void setValue(int a_value){ value = a_value; }
       int getValue(){ return value; }

       void setPathValue(int val){ pathValue = val;}
       int getPathValue(){ return pathValue; }

       void addEdge(Edge *edge);
       void removeEdge(Edge *edge);
       QList<Edge *> edges() const;

       enum { Type = UserType + 1 };
       int type() const { return Type; }

       void calculateForces();
       bool advance();
       QPainterPath shape() const;

       QRectF boundingRect() const;
       void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
        void rightClick(int);
        void leftClick(int);

        
    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    private:
        QList<Edge *> edgeList;
        QPointF newPos;
        MyQGraphicsView *graph;
        
};

#endif
