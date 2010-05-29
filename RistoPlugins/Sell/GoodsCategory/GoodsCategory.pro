# -------------------------------------------------
# GoodsCategory.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(GoodsCategory)
include(../../../common.pri)
HEADERS += goodscategory.h
OTHER_FILES += table.sql \
    function.sql
SOURCES += goodscategory.cpp
FORMS += goodscategorydetail.ui
