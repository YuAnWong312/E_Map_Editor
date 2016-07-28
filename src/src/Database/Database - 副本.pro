
TEMPLATE = app
#TEMPLATE = lib
TARGET = Database

include(Database.pri)
INCLUDEPATH += $$PWD/../../include/Database

#win32:LIBS += c:\mylibs\math.lib
#unix:LIBS += -lmath -L/usr/local/lib
#FORMS += ./myclass.ui
#RESOURCES += ./myclass.qrc

QT += core gui sql
CONFIG += debug_and_release qt
DEFINES += QT_CORE_LIB QT_GUI_LIB 
#DATABASE_DYNAMIC

CONFIG(debug, debug|release){
     TARGET = $${TARGET}d
     DESTDIR = ../../bin/
} else {
     TARGET = $$TARGET
     DESTDIR = ../../bin/
}
