

#FORMS += ./myclass.ui
#RESOURCES += ./myclass.qrc
#win32:LIBS += c:\mylibs\math.lib
#unix:LIBS += -lmath -L/usr/local/lib


target_name = MapShow
program_type = app
include_path = $$PWD/../../include/MapFramework
#QT += core gui
CONFIG += debug_and_release
#DEFINES += QT_CORE_LIB QT_GUI_LIB
include(MapShow.pri)
include(dependence.pri)
include(../macro.pri)
