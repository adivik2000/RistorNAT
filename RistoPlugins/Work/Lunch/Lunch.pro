# -------------------------------------------------
# Lunch.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Lunch)
include(../../../common.pri)

INCLUDEPATH += ../Util
LIBS += -L../ -lUtilWork

HEADERS += lunch.h
TRANSLATIONS += $$TARGET_it.ts
OTHER_FILES += database.txt
