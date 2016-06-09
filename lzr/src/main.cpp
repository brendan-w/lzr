#include "lzr.h"
#include <QApplication>
#include "inspect.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //load LZR's stylesheet
    QFile style_file("style.qss");
    style_file.open(QFile::ReadOnly);
    QString style = QLatin1String(style_file.readAll());
    a.setStyleSheet(style);

    LZR w;
    inspect(&w);
    w.show();

    return a.exec();
}
