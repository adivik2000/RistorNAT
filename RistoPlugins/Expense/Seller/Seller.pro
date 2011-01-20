# -------------------------------------------------
# Seller.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Sellers)
include(../../../common.pri)

HEADERS += seller.h
SOURCES += seller.cpp
FORMS += seller.ui
OTHER_FILES += table.sql function.sql \
    tests.sql
