# -------------------------------------------------
# Stock.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Stock)
include(../../../common.pri)

HEADERS += stock.h
SOURCES += stock.cpp
OTHER_FILES += database.txt
