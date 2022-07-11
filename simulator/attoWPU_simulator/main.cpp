#include <QtGui/QApplication>
#include "simulatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimulatorWindow w;
    // Prepare for reading keys
    a.installEventFilter(&w);
    w.show();

    return a.exec();
}
