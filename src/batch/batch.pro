#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

GT_MODULE_ID="Batch"

include( ../../settings.pri )

win32 {
    RC_FILE = batch.rc
}

TARGET = GTlabConsole

QT += core xml gui widgets network
TEMPLATE = app
CONFIG += console
CONFIG += silent
CONFIG += c++14

CONFIG(debug, debug|release){
    DESTDIR = $${BUILD_DEST}/debug-batch
    OBJECTS_DIR = $${BUILD_DEST}/debug-batch/obj
    MOC_DIR = $${BUILD_DEST}/debug-batch/moc
    RCC_DIR = $${BUILD_DEST}/debug-batch/rcc
    UI_DIR = $${BUILD_DEST}/debug-batch/ui
} else {
    DESTDIR = $${BUILD_DEST}/release-batch
    OBJECTS_DIR = $${BUILD_DEST}/release-batch/obj
    MOC_DIR = $${BUILD_DEST}/release-batch/moc
    RCC_DIR = $${BUILD_DEST}/release-batch/rcc
    UI_DIR = $${BUILD_DEST}/release-batch/ui
}
INCLUDEPATH += .\
    ../dataprocessor \
    ../dataprocessor/property \
    ../core \
    ../core/provider \
    ../core/process_management \
    ../core/process_management/process_runner \
    ../core/process_management/process_runner/commands \
    ../core/network \
    ../mdi \
    ../datamodel/property

DESTDIR = $${BUILD_DEST}

HEADERS += \
    batchremote.h \
    gt_consoleparser.h \
    gt_remoteprocessrunner.h \
    gt_remoteprocessrunnerstates.h

SOURCES += \
    batch.cpp \
    batchremote.cpp \
    gt_consoleparser.cpp \
    gt_remoteprocessrunner.cpp \
    gt_remoteprocessrunnerstates.cpp

LIBS += -L$${BUILD_DEST}

# Use of pre compiled logging header to reduce compile time
PRECOMPILED_HEADER = $${GTLAB_LOGGING_PATH}/include/logging/gt_logging.h

CONFIG(debug, debug|release){
    # UTILITIES
    LIBS += -lGTlabLogging-d
    # CORE
    LIBS += -lGTlabDataProcessor-d -lGTlabCore-d
    # THIRD PARTY
    win32 {
    }
    unix {
        contains(USE_HDF5, true) {
            LIBS += -lhdf5 -lhdf5_cpp
            LIBS += -lGenH5-d
        }
    }
} else {
    # UTILITIES
    LIBS += -lGTlabLogging
    # CORE
    LIBS += -lGTlabDataProcessor -lGTlabCore
    # THIRD PARTY
    unix {
        contains(USE_HDF5, true) {
            LIBS += -lhdf5 -lhdf5_cpp
            LIBS += -lGenH5
        }
    }
}

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
