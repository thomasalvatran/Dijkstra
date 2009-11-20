#include "MyQGraphicsView.h"

/*MyQGraphicsView::MyQGraphicsView(QWidget *ob, MainWindow* window=0): QGraphicsView(ob),mp_window(window)
{
    connect(this,SIGNAL(rightClick()),window, SLOT( showEditMenu()));
}*/

MyQGraphicsView::MyQGraphicsView(QWidget* ob): QGraphicsView(ob)
{
    
}

void MyQGraphicsView::mousePressEvent(QMouseEvent *e)
{
//    QMessageBox::information( NULL, "NOT IMPLEMENTED YET", QString("Function: ")+__FUNCTION__+"\nFile: "+__FILE__+"\nLine: "+__LINE__);
    if(e->button() == Qt::RightButton ) 
        emit rightClick(e);
    QGraphicsView::mousePressEvent(e);
}
