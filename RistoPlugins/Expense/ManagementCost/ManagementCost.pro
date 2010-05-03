# -------------------------------------------------
# Sellers.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(ManagementCost)
include(../../../common.pri)

HEADERS += managementcost.h
SOURCES += managementcost.cpp
FORMS += managementcost.ui
OTHER_FILES += table.sql function.sql
