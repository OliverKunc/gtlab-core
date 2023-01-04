#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/
######################################################################
#### DO NOT CHANGE BELOW !
######################################################################

GT_MODULE_ID="Test Mdi Interface"

include($${PWD}/../../../settings.pri)

BUILD_DEST = $${BUILD_DEST}/modules

CONFIG(debug, debug|release){
    TARGET = TestMdiInterface-d
} else {
    TARGET = TestMdiInterface
}

QT += core widgets xml svg

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

DEFINES += TEST_MDI_INTERFACE_DLL

isEmpty(GT_MODULE_ID) {
   error("GT_MODULE_ID undefined. Please define variable GT_MODULE_ID=\"My Module ID\" in project file.")
}

INCLUDEPATH += . \
    mdi \
    data

# Core
INCLUDEPATH += ../../../include/core
LIBS        += -L../../../lib/core
DEPENDPATH  += ../../../lib/core

DESTDIR = $${BUILD_DEST}

# Use of pre compiled logging header to reduce compile time
PRECOMPILED_HEADER = $${GTLAB_LOGGING_PATH}/include/logging/gt_logging.h

HEADERS += \
    data/test_mdi_data.h \
    data/test_mdi_package.h \
    mdi/test_mdi_dockwidget.h \
    mdi/test_mdi_packageui.h \
    mdi/test_mdi_viewer.h \
    test_mdi_interface_exports.h  \
    test_mdi_interface.h


SOURCES += \
    data/test_mdi_data.cpp \
    data/test_mdi_package.cpp \
    mdi/test_mdi_dockwidget.cpp \
    mdi/test_mdi_packageui.cpp \
    mdi/test_mdi_viewer.cpp \
    test_mdi_interface.cpp

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d
    LIBS += -lGTlabCore-d -lGTlabDataProcessor-d -lGTlabGui-d
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabCore -lGTlabDataProcessor -lGTlabGui
}

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
