# -------------------------------------------------
# Lunch.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Lunch)
include(../../../common.pri)

INCLUDEPATH += ../Util
LIBS += -L../Util -lUtilWork

HEADERS += lunch.h
TRANSLATIONS += $$TARGET_it.ts
OTHER_FILES += table.sql function.sql
