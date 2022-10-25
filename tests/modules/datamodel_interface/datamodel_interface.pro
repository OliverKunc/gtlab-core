#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

GT_MODULE_ID = "Test Datamodel Interface"

######################################################################
#### DO NOT CHANGE BELOW !
######################################################################

include($${PWD}/../../../settings.pri)

BUILD_DEST = $${BUILD_DEST}/modules

CONFIG(debug, debug|release){
    TARGET = TestDatamodelInterface-d
} else {
    TARGET = TestDatamodelInterface
}

QT += core widgets xml svg

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

DEFINES += TEST_DATAMODEL_INTERFACE_DLL

isEmpty(GT_MODULE_ID) {
   error("GT_MODULE_ID undefined. Please define variable GT_MODULE_ID=\"My Module ID\" in project file.")
}

DEFINES += GT_MODULE_ID='"\\\"$${GT_MODULE_ID}\\\""'

INCLUDEPATH += . \
    data

# Core
INCLUDEPATH += ../../../include/core
LIBS        += -L../../../lib/core
DEPENDPATH  += ../../../lib/core

DESTDIR = $${BUILD_DEST}

HEADERS += \
    test_datamodel_interface_exports.h  \
    test_datamodel_interface.h  \
    data/test_dmi_class.h \
    data/test_dmi_package.h


SOURCES += \
    test_datamodel_interface.cpp  \
    data/test_dmi_class.cpp \
    data/test_dmi_package.cpp

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d -lGTlabNumerics-d
    LIBS += -lGTlabCore-d -lGTlabDataProcessor-d
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging -lGTlabNumerics
    LIBS += -lGTlabCore -lGTlabDataProcessor
}

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
