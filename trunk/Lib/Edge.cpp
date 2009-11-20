/****************************************************************************
 **
 ** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: Qt Software Information (qt-info@nokia.com)
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License versions 2.0 or 3.0 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.GPL included in
 ** the packaging of this file.  Please review the following information
 ** to ensure GNU General Public Licensing requirements will be met:
 ** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
 ** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
 ** exception, Nokia gives you certain additional rights. These rights
 ** are described in the Nokia Qt GPL Exception version 1.3, included in
 ** the file GPL_EXCEPTION.txt in this package.
 **
 ** Qt for Windows(R) Licensees
 ** As a special exception, Nokia, as the sole copyright holder for Qt
 ** Designer, grants users of the Qt/Eclipse Integration plug-in the
 ** right for the Qt/Eclipse Integration to link to functionality
 ** provided by Qt Designer and its related libraries.
 **
 ** If you are unsure which license is appropriate for your use, please
 ** contact the sales department at qt-sales@nokia.com.
 **
 ****************************************************************************/

 #include <QPainter>

 #include "Edge.h"
 #include "Node.h"

 #include <math.h>

 static const double Pi = 3.14159265358979323846264338327950288419717;
 static double TwoPi = 2.0 * Pi;

 Edge::Edge(Node *sourceNode, Node *destNode,int val)
     : arrowSize(10) , value(val), retValue(-1), flag(false)
 {
     //setAcceptedMouseButtons(0);
     source = sourceNode;
     dest = destNode;
     source->addEdge(this);
     dest->addEdge(this);
     adjust();
 }

 Edge::~Edge()
 {
 }

 Node *Edge::sourceNode() const
 {
     return source;
 }

 void Edge::setSourceNode(Node *node)
 {
     source = node;
     adjust();
 }

 Node *Edge::destNode() const
 {
     return dest;
 }

 void Edge::setDestNode(Node *node)
 {
     dest = node;
     adjust();
 }

 void Edge::adjust()
 {
     if (!source || !dest)
         return;

     QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
     qreal length = line.length();
     QPointF edgeOffset((line.dx() * 30) / length, (line.dy() * 30) / length);

     prepareGeometryChange();
     sourcePoint = line.p1() + edgeOffset;
     destPoint = line.p2() - edgeOffset;
 }

 QRectF Edge::boundingRect() const
 {
     if (!source || !dest)
         return QRectF();

     qreal penWidth = 4;
     qreal extra = (penWidth + arrowSize) / 2.0;

     return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                       destPoint.y() - sourcePoint.y()))
         .normalized()
         .adjusted(-extra, -extra, extra, extra);
 }

 void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     if (!source || !dest)
         return;

     // Draw the line itself
     QLineF line(sourcePoint, destPoint);
     if( flag == true )
         painter->setPen(QPen(Qt::darkGreen, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     else
         painter->setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

     QFont font;
     font.setWeight(20);
     painter->setFont(font);
     painter->drawLine(line);

     float temp_x = destPoint.x() - sourcePoint.x();
     float temp_y = destPoint.y() - sourcePoint.y();
     float len = sqrt( temp_x*temp_x + temp_y*temp_y ); 
     float d_x = (  destPoint.x() - sourcePoint.x()  ) / len;
     float d_y = (  destPoint.y() - sourcePoint.y()  ) / len; 

     painter->setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

     if(retValue == -1)
         painter->drawText( destPoint.x() - 30* d_x ,
                 destPoint.y() - 30* d_y,
                 20,20, 
                 Qt::AlignCenter, 
                 QString::number(value));
     else
     {
         painter->drawText( destPoint.x() -30* d_x ,
                 destPoint.y() -30* d_y,
                 20,20, 
                 Qt::AlignCenter, 
                 QString::number(value));
         painter->drawText( sourcePoint.x() +30* d_x ,
                 sourcePoint.y() +30* d_y,
                 20,20, 
                 Qt::AlignCenter, 
                 QString::number(retValue));
     }


     // Draw the arrows if there's enough room
     double angle = ::acos(line.dx() / line.length());
     if (line.dy() >= 0)
         angle = TwoPi - angle;

     QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                   cos(angle + Pi / 3) * arrowSize);
     QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                   cos(angle + Pi - Pi / 3) * arrowSize);
     QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                               cos(angle - Pi / 3) * arrowSize);
     QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                               cos(angle - Pi + Pi / 3) * arrowSize);


     if(retValue != -1)
         painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
     painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
 }

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*if(event->button() == Qt::LeftButton ) 
    {
        emit leftClick(id);
        QGraphicsItem::mousePressEvent(event);
    }
    //else if( event->button() == Qt::MidButton ){
    */
    if( event->buttons() == Qt::RightButton )
    {
        emit rightClick(this);
        update();
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    bool ok;
    int val = QInputDialog::getInteger ( NULL, "Set value of node" , "Value" ,  10, 0, 2147483647, 1, &ok);
    if(!ok)
        return;
    value = val; 
    update();
    //}
    
    //update();
    //setPos( event->pos());
    //QGraphicsItem::mousePressEvent(event);
    
}

