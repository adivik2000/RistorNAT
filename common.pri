CONFIG += ppc
CONFIG += warn_on

TEMPLATE = lib
VERSION = 0.5.0

# Use a fast concatenation for QString and char*
DEFINES += QT_USE_FAST_CONCATENATION
DEFINES += QT_USE_FAST_OPERATOR_PLUS

# Use a branch predictor, by GCC
unix {
    QMAKE_CXX = "ccache g++"
    DEFINES += \'likely(x)=__builtin_expect((x),1)\'
    DEFINES += \'unlikely(x)=__builtin_expect((x),0)\'
} else {
    DEFINES += \'likely(x)=(x)\'
    DEFINES += \'unlikely(x)=(x)\'
}

ROOT = $$system(pwd)
DESTDIR="$$ROOT"/Plugins
OBJECTS_DIR = "$$ROOT"/.build/
MOC_DIR = "$$ROOT"/.build/
UI_DIR = "$$ROOT"/.build/
RCC_DIR = "$$ROOT"/.build/

#qManagementSoftware
INCLUDEPATH += "$$ROOT"/../qManagementSoftware/src
INCLUDEPATH += "$$ROOT"/core

LIBS += -L"$$ROOT"/../qManagementSoftware/src -lDatabase
