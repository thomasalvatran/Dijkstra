#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <qfile.h>
#include <qtextstream.h>

#include "../ui_MainWindow.h"
#include "../Algorithm/Algorithm.h"
#include "Node.h"
#include "Edge.h"
#define DEBUG 1
#ifdef DEBUG
    #include <iostream>
    using namespace std;
#endif



class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    public:
        MainWindow(QMainWindow *parent = 0);
        //void mousePressEvent(QMouseEvent *e);
        //void setBegin(int id);
        //void setEnd(int id);
    public slots:
        void newFile();
        void openFile();
        void saveFileAs();

        void undo();
        void addNode();
        void deleteNode();
        void connectNode();
        void editValue();
        void solve();
        void help();

        void edgeRightClick(Edge *edge);
        void nodeRightClick(int id);
        void nodeLeftClick(int id);
        void showEditMenu(QMouseEvent*);

    private:
        QGraphicsScene *scene;
        int node_id;
        QList<Node*> nodes;
        QList<Edge*> edges;
        bool f_connect;
        void clear();
        Node* getNode(int id);
        Edge* getEdge(int source_id,int dest_id, int &rec);
        Edge* getEdge(int source_id,int dest_id);
        
};
#endif

