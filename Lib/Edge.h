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

#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QInputDialog>

#include <QGraphicsSceneMouseEvent>
#include <cmath>
//class Node
//class Edge;
#include "MyQGraphicsView.h"

class Node;

class Edge : public QObject, public QGraphicsItem
{
    Q_OBJECT
    public:
        Edge(Node *sourceNode, Node *destNode,int val=10);
        ~Edge();

        void set(){ flag = true; }
        void unSet(){ flag = false; }
        void setValue(int val){value=val;}
        int getValue(){return value;}
        void setRetValue(int val){retValue = val;}
        int getRetValue(){return retValue;}
        Node *sourceNode() const;
        void setSourceNode(Node *node);

        Node *destNode() const;
        void setDestNode(Node *node);

        void adjust();

        enum { Type = UserType + 2 };
        int type() const { return Type; }

    protected:
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
    private:
        Node *source, *dest;

        QPointF sourcePoint;
        QPointF destPoint;
        qreal arrowSize;
        int value;
        int retValue;
        int flag;
    signals:
        void rightClick(Edge *self);
};

#endif

