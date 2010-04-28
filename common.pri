CONFIG += ppc
CONFIG += warn_on

TEMPLATE = lib
VERSION = 0.5.0

# Use a fast concatenation for QString and char*
DEFINES += QT_USE_FAST_CONCATENATION
DEFINES += QT_USE_FAST_OPERATOR_PLUS

# Use a branch predictor, by GCC
unix {
    DEFINES += \'likely(x)=__builtin_expect((x),1)\'
    DEFINES += \'unlikely(x)=__builtin_expect((x),0)\'
} else {
    DEFINES += \'likely(x)=(x)\'
    DEFINES += \'unlikely(x)=(x)\'
}

ROOT = $$system(pwd)
DESTDIR="$$ROOT"/Plugins

#qManagementSoftware
INCLUDEPATH += "$$ROOT"/../qManagementSoftware/
INCLUDEPATH += "$$ROOT"/core

LIBS += -L"$$ROOT"/../qManagementSoftware/Library -lDatabase
