# -------------------------------------------------
# Seller.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Sellers)
include(../../../common.pri)

HEADERS += seller.h
SOURCES += seller.cpp
FORMS += seller.ui
OTHER_FILES += database.txt
