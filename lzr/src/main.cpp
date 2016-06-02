#include "lzr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //load LZR's stylesheet
    QFile style_file("style.qss");
    style_file.open(QFile::ReadOnly);
    QString style = QLatin1String(style_file.readAll());
    a.setStyleSheet(style);

    LZR w;
    w.show();

    return a.exec();
}
