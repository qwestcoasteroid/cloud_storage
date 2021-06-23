#include "mainwindow.h"

#include "authorizationwindow.h"

#include <QApplication>
#include <optional>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //MainWindow w;
    AuthorizationWindow w;
    w.show();
    return a.exec();
}
