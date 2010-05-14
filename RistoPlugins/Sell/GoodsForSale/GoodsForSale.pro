# -------------------------------------------------
# GoodsForSale.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(GoodsForSale)
include(../../../common.pri)
HEADERS += goodsforsale.h \
    detail.h
SOURCES += goodsforsale.cpp \
    detail.cpp
FORMS += detail.ui
OTHER_FILES += table.sql \
    function.sql
