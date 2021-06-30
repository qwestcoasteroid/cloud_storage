QT -= gui

QT += network sql

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        connection.cpp \
        connectionhandler.cpp \
        databaseconnection.cpp \
        main.cpp \
        networklistener.cpp \
        networkreader.cpp \
        networkwriter.cpp \
        ntconfig.cpp \
        protocol.cpp \
        sessionsmanager.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    connection.h \
    connectionhandler.h \
    databaseconnection.h \
    networklistener.h \
    networkreader.h \
    networkwriter.h \
    ntconfig.h \
    protocol.h \
    sessionsmanager.h

DISTFILES += \
    migration.sql
