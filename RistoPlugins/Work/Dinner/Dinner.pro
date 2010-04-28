# -------------------------------------------------
# Dinner.pro
# -------------------------------------------------
TARGET = $$qtLibraryTarget(Dinner)
include(../../../common.pri)

INCLUDEPATH += ../Util
LIBS += -L../ -lUtilWork

HEADERS += dinner.h
TRANSLATIONS += $$TARGET_it.ts
OTHER_FILES += database.txt
