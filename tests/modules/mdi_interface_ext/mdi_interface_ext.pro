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
    TARGET = TestMdiInterfaceExt-d
} else {
    TARGET = TestMdiInterfaceExt
}

QT += core widgets xml svg

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

INCLUDEPATH += . \
    data \
    mdi \
    ../mdi_interface \
    ../mdi_interface/data

# Core
INCLUDEPATH += ../../../include/core
LIBS        += -L../../../lib/core
DEPENDPATH  += ../../../lib/core

LIBS        += -L$${BUILD_DEST}
DEPENDPATH  += $${BUILD_DEST}

DESTDIR = $${BUILD_DEST}

HEADERS += \
    mdi/test_mdi_ext_packageui.h \
    data/test_mdi_ext_package.h \
    mdi/test_mdi_ext_viewer.h \
    test_mdi_interface_ext.h


SOURCES += \
    mdi/test_mdi_ext_packageui.cpp \
    data/test_mdi_ext_package.cpp \
    mdi/test_mdi_ext_viewer.cpp \
    test_mdi_interface_ext.cpp \

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d -lGTlabNumerics-d
    LIBS += -lGTlabCore-d -lGTlabDataProcessor-d -lGTlabGui-d
    LIBS += -lTestMdiInterface-d
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging -lGTlabNumerics
    LIBS += -lGTlabCore -lGTlabDataProcessor -lGTlabGui
    LIBS += -lTestMdiInterface
}

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
