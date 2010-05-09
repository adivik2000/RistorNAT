# -------------------------------------------------
# Sellers.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(ManagementCost)
include(../../../common.pri)
HEADERS += managementcost.h \
    updateamountdelegate.h
SOURCES += managementcost.cpp \
    updateamountdelegate.cpp
FORMS += managementcost.ui
OTHER_FILES += table.sql \
    function.sql
