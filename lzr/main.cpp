#include "lzr_editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LZREditor w;
    w.show();

    return a.exec();
}
