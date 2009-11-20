#include <QtGui>

#include "MainWindow.h"

MainWindow::MainWindow( QMainWindow *parent): QMainWindow(parent)
{
    setupUi(this);

    connect(actionNew ,SIGNAL(triggered()),this, SLOT( newFile()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSaveAs,SIGNAL(triggered()),this, SLOT( saveFileAs()));

    connect(actionUndo,SIGNAL(triggered()),this, SLOT( undo()));
    connect(actionAddNode,SIGNAL(triggered()),this, SLOT( addNode()));
    connect(actionDeleteNode,SIGNAL(triggered()),this, SLOT( deleteNode() ));
    connect(actionConnect,SIGNAL(triggered()),this, SLOT( connectNode()));
    connect(actionSolve,SIGNAL(triggered()),this, SLOT( solve()));
    connect(actionHelp,SIGNAL(triggered()),this, SLOT( help()));
    connect(actionEditValue,SIGNAL(triggered()),this, SLOT( editValue()));

    scene = new QGraphicsScene();
    graph->setScene(scene);

    f_connect = false;
}

void MainWindow::clear(){
    foreach(Edge *e,edges)
    {
        scene->removeItem(e);
        delete e;
    }
    edges.clear();
    foreach(Node *n,nodes)
    {
        scene->removeItem(n);
        delete n;
    }
    Node::resetId();
    nodes.clear();
}

Node* MainWindow::getNode(int id)
{
    foreach(Node * n,nodes)
        if( n->getId() == id )
           return n; 
    return NULL;
}

Edge* MainWindow::getEdge(int source_id,int dest_id, int &rec)
{
    foreach( Edge * e, edges)
    {
        if( e->sourceNode()->getId() == source_id &&
                e->destNode()->getId() == dest_id )
        {
            rec = true;
            return e;
        }
        else if( e->sourceNode()->getId() == dest_id &&
                e->destNode()->getId() == source_id )
        {
            rec = false;
            return e;
        }
    }
}

Edge* MainWindow::getEdge(int source_id,int dest_id)
{
    foreach( Edge * e, edges)
    {
        if( e->sourceNode()->getId() == source_id &&
                e->destNode()->getId() == dest_id )
            return e;
        else if( e->sourceNode()->getId() == dest_id &&
                e->destNode()->getId() == source_id )
            return e;
    }
}
void MainWindow::newFile()
{
    if( QMessageBox::Yes ==
            QMessageBox::question ( this,
                "New file", 
                "Do you realy want to open new file",
                QMessageBox::Yes, QMessageBox::Cancel)  )
    clear();
}
void MainWindow::openFile()
{
    clear();
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("Text File(*.dijkstra)"));
    QFile f( s );

    if( !f.open( QIODevice::ReadOnly) )
    {
        QMessageBox::information( this, "Error",tr("File can't be opened")); 
        return;
    }

    QTextStream ts( &f );

    int size; 
    ts>>size;
    cout<<size<<endl;

    int ** array = new int*[ size ];
    for(int i = 0; i < size ; i++)
    {
        array[i] = new int [ size ];
        for( int j = 0 ; j < size ; j++ )
            array[i][j] = -1;
    }

    for(int i = 0; i < size ; i++)
    {
        for( int j = 0 ; j < size ; j++ )
        {
            ts>>array[i][j];
            #if DEBUG
                cout<<array[i][j]<<endl;
            #endif
        }
    }
    for( int i = 0; i < size ; i++)
    {
        int x;
        int y;
        ts>> x;
        ts>> y;
        Node *node = new Node(graph,array[i][i]);
        node->setPos(x,y);
        nodes<<node;
        scene->addItem(node);
    }
    for(int i = 0; i < size ; i++)
    {
        for( int j = 0 ; j < size ; j++ )
        {
            if( i == j || array[i][j] == -1)
                continue;
            Node *s = getNode(i);
            Node *d = getNode(j);

            Edge* temp = getEdge(j,i);
            if( temp != NULL )
            {
                temp -> setRetValue(array[i][j]);
                continue;
            }
            cout<<"d"<<endl;
            Edge *e = new Edge(s,d,array[i][j]);
            edges<<e;
            scene->addItem(e);

            cout<<"e"<<endl;
            //ts>>array[i][j];
            cout<<array[i][j]<<endl;
        }
    }
    f.close();
}
void MainWindow::saveFileAs()
{
    QString s = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                 "",
                                                 tr("Text File(*.dijkstra)"));
    QFile f( s );

    if( !f.open( QIODevice::WriteOnly ) )
    {
        QMessageBox::information( this, "Error",tr("File can't be saved")); 
        return;
    }

    QTextStream ts( &f );

    int size = Node::getLastId();
    ts << size<<endl;

    int ** array = new int*[ size ];
    for(int i = 0; i < size ; i++)
    {
        array[i] = new int [ size ];
        for( int j = 0 ; j < size ; j++ )
            array[i][j] = -1;
    }

    foreach( Node* n, nodes)
        array[n->getId()][n->getId()] = n->getValue();
    foreach( Edge* e, edges)
    {
        array [(e->sourceNode())->getId() ]
            [ (e->destNode())->getId() ] 
            = e-> getValue(); 
        if(e->getRetValue() != -1)
        array[ (e->destNode())->getId() ] 
            [(e->sourceNode())->getId() ] 
            = e-> getRetValue(); 
    }

    for(int i = 0; i < size ; i++)
    {
        for( int j = 0 ; j < size ; j++ )
            ts<<array[i][j]<<" ";
        ts<<endl;
    }
    foreach(Node *n, nodes)
        ts<< n->pos().x()<<" "<< n->pos().y()<<endl;
    f.close();
}

void MainWindow::undo()
{
    QMessageBox::information( this, "NOT IMPLEMENTED YET", QString("Function: ")+__FUNCTION__+"\nFile: "+__FILE__+"\nLine: "+__LINE__);
}
void MainWindow::addNode()
{
    QPoint pos = menuEdycja->pos();
    bool ok;
    int value = QInputDialog::getInteger ( this , tr("Set value of node") , tr("Value" ),  10, 0, 2147483647, 1, &ok);
        if(!ok)
            return;
    Node* newNode = new Node(graph,value);
    newNode->setPos(pos);
    nodes << newNode;
    scene->addItem(newNode);
    connect(newNode,SIGNAL(rightClick(int)),this,SLOT(nodeRightClick(int)));
    connect(newNode,SIGNAL(leftClick(int)),this,SLOT(nodeLeftClick(int)));
}

void MainWindow::deleteNode()
{
    foreach(Node* node, nodes)
        if(node->getId() == node_id)
        {
            QList<Edge *> list = node->edges();
            foreach( Edge *ed, list)
            {
                (ed->sourceNode())->removeEdge(ed);
                (ed->destNode())->removeEdge(ed);
                delete ed;
            }
            nodes.removeOne(node);
            delete node;
        }
}

void MainWindow::connectNode()
{
    f_connect = true;
    
}

void MainWindow::solve(){
    bool ok;
    int start = QInputDialog::getInteger ( this , tr("Set start node") , tr("Value" ),  0, 0, 2147483647, 1, &ok);
    if(!ok)
        return;
    int end = QInputDialog::getInteger ( this , tr("Set end node") , tr("Value" ),   Node::getLastId()-1, 0, Node::getLastId()-1, 1, &ok);
    if(!ok)
        return;

    int size = Node::getLastId();
    int ** array = new int*[ size ];
    for(int i = 0; i < size ; i++)
    {
        array[i] = new int [ size ];
        for( int j = 0 ; j < size ; j++ )
            array[i][j] = -1;
    }

    foreach( Node* n, nodes)
        array[n->getId()][n->getId()] = n->getValue();
    foreach( Edge* e, edges)
    {
        array[(e->sourceNode())->getId()][ (e->destNode())->getId() ] = e-> getValue(); 
        if(e->getRetValue() != -1)
        array[ (e->destNode())->getId() ] [(e->sourceNode())->getId()] = e-> getRetValue(); 
    }

    Algorithm dij(array,size);
    for(int i = 0; i < size ; i++)
    {
        for( int j = 0 ; j < size ; j++ )
            cout<<array[i][j]<<" ";
        cout<<endl;
    }

    int *value;
    int *prev;
    if( dij.resolve(start,end,prev, value) == false)
    {
        QMessageBox::information( this, "Error", "This graph don't have solutions."); 
    }
    else
    {
        foreach(Node* n, nodes)
        {
            n->setPathValue(-1);
            n->update();
        }
        foreach(Edge* e, edges)
        {
                e->unSet();
                e->update();
        }
        for(int i = 0 ; i < size ; i ++)
        {
            foreach(Node* n, nodes)
            {
                if( n->getId() == prev[i] )
                {
                    n->setPathValue(value[i]);
                    n->update();
                }
            }
            if( i < size -1)
                foreach(Edge* e, edges)
                {
                    if( e->sourceNode()->getId() == prev[i]
                        && e->destNode()->getId() == prev[i+1])
                    {
                        e->set();
                        e->update();
                    }
                    if( e->sourceNode()->getId() == prev[i+1]
                        && e->destNode()->getId() == prev[i])
                    {
                        e->set();
                        e->update();
                    }
                }
            if( prev[i] == start )
                break;
        }
    }
}



void MainWindow::help(){
    QMessageBox::information( this, "Help", tr("Created by Piotr Wilk"));
}

void MainWindow::edgeRightClick(Edge * edge)
{
    edges.removeOne(edge);
    //delete edge; 
    scene->removeItem(edge);
}

void MainWindow::nodeRightClick(int id)
{
    actionDeleteNode->setEnabled(true);
    actionConnect->setEnabled(true);
    actionEditValue->setEnabled(true);
    node_id=id;
    menuEdycja->show();
}

void MainWindow::nodeLeftClick(int id)
{
    if(f_connect)
    {
        f_connect = false;
        bool ok;
        int value;

        if(id == node_id || id < 0 || node_id < 0)
        {
            return;
        }

        Node *source;
        Node *destination;

        foreach(Node* node, nodes)
        {
            if(node->getId() == node_id)
                source = node;
            if(node->getId() == id)
                destination = node;
        }
        foreach(Edge* edge, edges)
        {   
            if(edge->sourceNode() == source && edge->destNode() == destination)
            {
                value = QInputDialog::getInteger ( this , tr("Change value of edge") , tr("Value" ),  10, 0, 2147483647, 1, &ok);
                if(!ok)
                    return;
                edge->setValue(value);
                edge->update();
                return;
            }
            if(edge->sourceNode() == destination && edge->destNode() == source)
            {

                value = QInputDialog::getInteger ( this , tr("Set return value of edge") , tr("Value" ),  10, 0, 2147483647, 1, &ok);
                if(!ok) return;
                edge->setRetValue(value);
                edge->update();
                return;
            }
        }

        value = QInputDialog::getInteger ( this , tr("Set value of edge") , tr("Value" ),  10, 0, 2147483647, 1, &ok);
        if(!ok)
            return;
        Edge* temp = new Edge(source,destination,value);
        connect(temp,SIGNAL(rightClick(Edge *)),this,SLOT(edgeRightClick(Edge *)));
        //connect(newNode,SIGNAL(leftClick(int)),this,SLOT(nodeLeftClick(int)));
        edges << temp;
        scene->addItem(temp);
    }
}

void MainWindow::editValue(){
    bool ok;
    int value = QInputDialog::getInteger ( this , tr("Set value of node") , tr("Value" ),  10, 0, 2147483647, 1, &ok);
        if(!ok)
            return;
        foreach( Node * node, nodes)
        {    
            if(node->getId() == node_id)
                node->setValue(value);
            node->update ();
        }
}

void MainWindow::showEditMenu( QMouseEvent* e)
{
    actionDeleteNode->setEnabled(false);
    actionConnect->setEnabled(false);
    actionEditValue->setEnabled(false);
    menuEdycja->popup(e->globalPos());
}
