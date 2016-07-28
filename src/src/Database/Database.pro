
target_name = Database
program_type = lib
include_path = $$PWD/../../include
QT += core gui sql
CONFIG += debug_and_release
DEFINES += DATABASE_DYNAMIC
#QT_CORE_LIB QT_GUI_LIB
include(DataBase.pri)
include(dependence.pri)
include(../macro.pri)
