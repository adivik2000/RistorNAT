# -------------------------------------------------
# core.pro
# -------------------------------------------------
TARGET = RistorNAT
include(../common.pri)
TEMPLATE = app
DESTDIR = "$$ROOT"
SOURCES += main.cpp \
    mainwindow.cpp \
    stackedwcontainer.cpp \
    settings.cpp \
    settingsui.cpp
HEADERS += mainwindow.h \
    settings.h \
    settingsui.h \
    stackedwcontainer.h \
    plugininterface.h
FORMS += mainwindow.ui \
    about.ui \
    settingsui.ui
RESOURCES += res.qrc
TRANSLATIONS += core_it.ts
ICON = ristornat.icns
OTHER_FILES += database.txt
