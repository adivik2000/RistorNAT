# -------------------------------------------------
# SellCategory.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(SellCategory)
include(../../../common.pri)
HEADERS += sellcategory.h
OTHER_FILES += table.sql \
    function.sql
SOURCES += sellcategory.cpp
FORMS += sellcategorydetail.ui
