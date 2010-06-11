## RistorNAT configuration

# Set to true to build tests.
MAKE_TEST = false

## End of RistorNAT configuration

CONFIG += ppc
CONFIG += warn_on
ROOT = $$system(pwd)

contains(MAKE_TEST, true) {
	QT += testlib
} else {
	TEMPLATE = lib
	VERSION = 0.5.0
	DESTDIR="$$ROOT"/Plugins
}

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

#qManagementSoftware
INCLUDEPATH += "$$ROOT"/../qManagementSoftware/src
INCLUDEPATH += "$$ROOT"/core

OBJECTS_DIR = "$$ROOT"/.build/
MOC_DIR = "$$ROOT"/.build/
UI_DIR = "$$ROOT"/.build/
RCC_DIR = "$$ROOT"/.build/

LIBS += -L"$$ROOT"/../qManagementSoftware/src -lDatabase


