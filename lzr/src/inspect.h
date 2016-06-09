
#pragma once

#include <QString>
#include <QObject>
#include <QDebug>

#ifndef SPACER
#  define SPACER "    "
#endif

static void print_qobject(QObject* o, int level)
{
    QString line;
    while((level--) != 0) line += SPACER;

    line += o->metaObject()->className();
    if(o->objectName().length() > 0)
        line += " #" + o->objectName();

    qDebug() << line.toUtf8().constData();
}

void inspect(QObject* o, int level=0)
{
    if(level == 0) print_qobject(o, level);

    level++;

    foreach(QObject* child, o->children())
    {
        print_qobject(child, level);
        inspect(child, level); //recurse
    }
}
