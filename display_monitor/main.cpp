#include <QApplication>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "query_data.h"
#include "widget.h"
#include "client/rpc_client.h"
#include "mprpcapplication.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");
    qRegisterMetaType<char**>("char**");
    Widget w(argc, argv);
    w.show();
    return a.exec();
}
