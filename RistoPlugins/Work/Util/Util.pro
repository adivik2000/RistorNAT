# -------------------------------------------------
# Util.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(UtilWork)
include(../../../common.pri)

CONFIG += static
DESTDIR = ../

HEADERS += workui.h
SOURCES += workui.cpp
FORMS += workui.ui
TRANSLATIONS += $$TARGET_it.ts
