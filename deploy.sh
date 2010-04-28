#!/bin/bash

PACKAGE="RistorNAT.app"
VERSION=0

echo -en "Modifying for deploy..\n"

function install_libDB() {
        install_name_tool -change libDatabase.$VERSION.dylib \
                @executable_path/../Library/libDatabase.$VERSION.dylib \
                $1
}

function install_libUserInterface() {
        install_name_tool -change libUserInterface.$VERSION.dylib \
                @executable_path/../Library/libUserInterface.$VERSION.dylib \
                $1
}

rm -rf $PACKAGE/Contents/Library
rm -rf $PACKAGE/Contents/Plugins

mkdir -p $PACKAGE/Contents/Database
mkdir -p $PACKAGE/Contents/Plugins

echo -en "Created package structure..\n"

cp Database/DatabaseSchema.sql \
   Database/DatabaseCreation.sql \
   Database/data_example_dump.sql \
        $PACKAGE/Contents/Database

echo -en "Database information copied..\n"

cp -r ../qManagementSoftware/Library $PACKAGE/Contents
cp -r Plugins $PACKAGE/Contents

echo -en "Package structure filled..\n"

cd $PACKAGE/Contents

for i in `find . -type f | grep dylib`; do
    install_libUserInterface $i
    install_libDB $i
done

echo `pwd`
install_libDB MacOS/RistorNAT
install_libUserInterface MacOS/RistorNAT

echo "Done."
