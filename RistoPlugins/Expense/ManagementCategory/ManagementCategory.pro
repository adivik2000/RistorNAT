# -------------------------------------------------
# MngmCostReport.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(ManagementCategory)
include(../../../common.pri)

HEADERS += managementcategory.h
SOURCES += managementcategory.cpp
FORMS += managementcategory.ui
OTHER_FILES += database.txt
