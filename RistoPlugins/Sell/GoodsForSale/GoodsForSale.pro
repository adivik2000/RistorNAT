# -------------------------------------------------
# GoodsForSale.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(GoodsForSale)
include(../../../common.pri)
HEADERS += goodsforsale.h
SOURCES += goodsforsale.cpp
FORMS += goodsforsale.ui
OTHER_FILES += table.sql \
    function.sql
