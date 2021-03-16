QT += core gui
QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = chorespp
TEMPLATE = app
VERSION = 1.9
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    combo.cpp \
    dialog.cpp \
    form.cpp \
    main.cpp \
    mainwindow.cpp \
    runguard.cpp

HEADERS += \
    combo.h \
    dialog.h \
    form.h \
    global.h \
    mainwindow.h \
    runguard.h

FORMS += \
    combo.ui \
    dialog.ui \
    form.ui \
    mainwindow.ui

RESOURCES += \
    chorespp.qrc

unix:!android {
    target.path = $${PREFIX}/bin
    icon.path = /usr/share/pixmaps
    icon.files = icons/chorespp.png
    desktop.path = /usr/share/applications
    desktop.files = chorespp.desktop
    INSTALLS += target icon desktop
}

win32 {
    VERSION = $${VERSION}.0.0
    QMAKE_TARGET_COMPANY = DP Software
    QMAKE_TARGET_DESCRIPTION = A simple and very lightweight to-do app
    QMAKE_TARGET_COPYRIGHT = \\251 2020 Dimitris Psathas
    QMAKE_TARGET_PRODUCT = Chores
    RC_ICONS = icons/chorespp.ico
    RC_LANG = 0x408
    RC_CODEPAGE = 1253
}

DISTFILES += \
    chorespp.desktop
