# -------------------------------------------------
# Project created by QtCreator 2010-01-15T23:46:34
# -------------------------------------------------
TARGET = QLedIndicatorPlugin
TEMPLATE = lib
CONFIG += designer \
    plugin \
    debug_and_release
DEFINES += QLEDINDICATORPLUGIN_LIBRARY
SOURCES += qledindicatorplugin.cpp \
    ../qledindicator.cpp
HEADERS += qledindicatorplugin.h \
    QLedIndicatorPlugin_global.h \
    ../qledindicator.h
