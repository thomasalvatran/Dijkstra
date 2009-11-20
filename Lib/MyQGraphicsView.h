#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMessageBox>
#include <QMouseEvent>
//#include "MainWindow.h"

//class MainWindow;

class MyQGraphicsView: public QGraphicsView
{
    Q_OBJECT
    public:
    MyQGraphicsView(QWidget *ob);
    void mousePressEvent(QMouseEvent *e);
    

    public slots:

    signals:
    void rightClick(QMouseEvent*);
};


#endif
