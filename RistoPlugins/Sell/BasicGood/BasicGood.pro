# -------------------------------------------------
# BasicGood.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(BasicGood)
include(../../../common.pri)
HEADERS += basicgood.h
OTHER_FILES += table.sql function.sql
SOURCES += basicgood.cpp
