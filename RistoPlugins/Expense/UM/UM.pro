# -------------------------------------------------
# UM.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(UM)
include(../../../common.pri)
HEADERS += um.h
SOURCES += um.cpp
OTHER_FILES += table.sql \
    function.sql
