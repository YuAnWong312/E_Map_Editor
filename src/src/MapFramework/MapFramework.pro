

#win32:LIBS += c:\mylibs\math.lib
#unix:LIBS += -lmath -L/usr/local/lib
#FORMS += ./myclass.ui
#RESOURCES += ./myclass.qrc

target_name = MapFramework
program_type = lib
include_path = $$PWD/../../include/MapFramework
QT += core gui xml opengl sql
CONFIG += debug_and_release
DEFINES += MAPFRAMEWORK_DYNAMIC
#QT_CORE_LIB QT_GUI_LIB
include(MapFramework.pri)
include(dependence.pri)
include(../macro.pri)
