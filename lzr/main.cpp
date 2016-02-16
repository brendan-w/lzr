#include "frameeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrameEditor w;
    w.show();

    return a.exec();
}
