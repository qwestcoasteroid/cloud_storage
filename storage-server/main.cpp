#include <QCoreApplication>

#include "networklistener.h"
#include "connectionhandler.h"

#include <iostream>

constexpr quint16 kDefaultPort = 56565;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    NetworkListener listener(QHostAddress::Any, kDefaultPort);

    return a.exec();
}
