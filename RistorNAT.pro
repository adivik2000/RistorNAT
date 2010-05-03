TEMPLATE = subdirs
SUBDIRS = core \
    RistoPlugins
OTHER_FILES += common.pri \
    script/osx/deploy.sh \
    script/linux/RistorNAT.sh \
    script/gen_dbscript.sh
QMAKE_CLEAN += Plugins/*
QMAKE_DISTCLEAN += .build Plugins
