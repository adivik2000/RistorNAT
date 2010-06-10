# -------------------------------------------------
# ManagementCategory.pro
# -------------------------------------------------
include(../../../common.pri)

contains(MAKE_TEST, true) {
	TARGET = "Test"
	SOURCES = testmanagementcategory.cpp
} else {
	TARGET = $$qtLibraryTarget(ManagementCategory)
	HEADERS += managementcategory.h
	SOURCES += managementcategory.cpp
	FORMS += managementcategory.ui
	OTHER_FILES += table.sql function.sql
}

