QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authorizationrequest.cpp \
    authorizationwindow.cpp \
    cipher.cpp \
    connection.cpp \
    deletefilerequest.cpp \
    downloadfilerequest.cpp \
    filereader.cpp \
    filewriter.cpp \
    irequest.cpp \
    main.cpp \
    mainwindow.cpp \
    networkreader.cpp \
    networkwriter.cpp \
    ntconfig.cpp \
    profilerequest.cpp \
    protocol.cpp \
    registrationrequest.cpp \
    storagecontentrequest.cpp \
    storageinfo.cpp \
    tariffsrequest.cpp \
    transferitem.cpp \
    uploadfilerequest.cpp

HEADERS += \
    authorizationrequest.h \
    authorizationwindow.h \
    cipher.h \
    connection.h \
    deletefilerequest.h \
    downloadfilerequest.h \
    filereader.h \
    filewriter.h \
    irequest.h \
    mainwindow.h \
    networkreader.h \
    networkwriter.h \
    ntconfig.h \
    profilerequest.h \
    protocol.h \
    registrationrequest.h \
    storagecontentrequest.h \
    storageinfo.h \
    tariffsrequest.h \
    transferitem.h \
    uploadfilerequest.h

FORMS += \
    authorizationwindow.ui \
    mainwindow.ui \
    transferitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# CryptoPP library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/cryptopp850/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/cryptopp850/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/cryptopp850/ -lcryptopp

INCLUDEPATH += $$PWD/cryptopp850
DEPENDPATH += $$PWD/cryptopp850
