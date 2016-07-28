
target_name = MapEditor
program_type = app
include_path = $$PWD/../../include/MapFramework
#QT += core gui
CONFIG += debug_and_release
#DEFINES += QT_CORE_LIB QT_GUI_LIB
include(MapEditor.pri)
include(dependence.pri)
include(../macro.pri)
