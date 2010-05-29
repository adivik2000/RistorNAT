# -------------------------------------------------
# GoodsForSale.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(GoodsForSale)
include(../../../common.pri)
HEADERS += goodsforsale.h
SOURCES += goodsforsale.cpp
FORMS += 
OTHER_FILES += table.sql \
    function.sql
