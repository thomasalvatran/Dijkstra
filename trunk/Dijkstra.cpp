#include <QApplication>
#include "Lib/MainWindow.h"

int Node::inc_id = 0;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow* Dijkstra = new MainWindow();
    Dijkstra->show();
    return app.exec();
}

