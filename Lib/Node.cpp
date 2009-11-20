#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMessageBox>

#include "Node.h"

Node::Node(MyQGraphicsView *pointer,int  val,int a_size): id(inc_id++),value(val),size(a_size), half_size(a_size/2),pathValue(-1),graph(pointer)
{
    setFlag(ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

Node::~Node()
{
 /* foreach(Edge *edge, edgeList)
  {
      (edge->sourceNode());
      (edge->destNode());
      delete edge;
  }
  */
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    edgeList.removeOne(edge);
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}
void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
        qreal dx = line.dx();
        qreal dy = line.dy();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList) {
        QPointF pos;
        if (edge->sourceNode() == this)
            pos = mapFromItem(edge->destNode(), 0, 0);
        else
            pos = mapFromItem(edge->sourceNode(), 0, 0);
        xvel += pos.x() / weight;
        yvel += pos.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-(half_size) - adjust, -(half_size) - adjust,
            size + adjust+2, size + adjust+2);
}
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-half_size, -half_size, size, size);
    return path;
}
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::SolidLine);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-half_size, -half_size, size, size);
    QRadialGradient gradient(-3, -3, 20);

    if( pathValue != -1 )
    {    
        //gradient.setCenter(3, 3);
        //gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::darkGreen).light(120));
        gradient.setColorAt(0, QColor(Qt::green).light(120));
    }
    else if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-half_size, -half_size, size, size);

    if( pathValue == -1 )
        painter->drawText(QRectF(-half_size,-half_size,size,size), Qt::AlignCenter, QString::number(id)+"( "+QString::number(value)+" )");
    else
        painter->drawText(QRectF(-half_size,-half_size,size,size), Qt::AlignCenter, QString::number(id)+"( "+QString::number(value)+" )\n"+ QString::number(pathValue));

    
}
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    
    switch (change) {
        case ItemPositionHasChanged:
            foreach (Edge *edge, edgeList)
                edge->adjust();
            //graph->itemMoved();
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
   
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        update();
    if(event->button() == Qt::LeftButton ) 
    {
        emit leftClick(id);
        QGraphicsItem::mousePressEvent(event);
    }
    //else if( event->button() == Qt::MidButton ){
    else if( event->button() == Qt::RightButton ){
        emit rightClick(id);
        //QGraphicsItem::mousePressEvent(event);
    }
    //QMessageBox::information( NULL, "Application name",
    //        "Lalala.\n"
    //        "alalalal" );
    
    //update();
    //setPos( event->pos());
    //QGraphicsItem::mousePressEvent(event);
    
}
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
         update();
    QGraphicsItem::mouseReleaseEvent(event);
}
