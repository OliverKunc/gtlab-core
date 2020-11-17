#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include($${PWD}/../../../settings.pri)

BUILD_DEST = ../../../build/modules

CONFIG(debug, debug|release){
    TARGET = testDatamodelInterface-d
} else {
    TARGET = testDatamodelInterface
}

QT += core widgets xml svg

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++11

DEFINES += TEST_DATAMODEL_INTERFACE_DLL

INCLUDEPATH += . \

# Core
INCLUDEPATH += ../../../include/core
LIBS        += -L../../..//lib/core
DEPENDPATH  += ../../../lib/core

DESTDIR = $${BUILD_DEST}

HEADERS += \
    test_datamodel_interface_exports.h  \
    test_datamodel_interface.h  \
    test_dmi_class.h \
    test_dmi_package.h \
    test_dmi_packageui.h


SOURCES += \
    test_datamodel_interface.cpp  \
    test_dmi_class.cpp \
    test_dmi_package.cpp \
    test_dmi_packageui.cpp

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d -lGTlabDatamodel-d -lGTlabNumerics-d -lGTlabCalculators-d
    LIBS += -lGTlabCore-d -lGTlabMdi-d -lGTlabNetwork-d
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging -lGTlabDatamodel -lGTlabNumerics -lGTlabCalculators
    LIBS += -lGTlabCore -lGTlabMdi -lGTlabNetwork
}
