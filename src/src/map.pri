
include(path.pri)
INCLUDEPATH += $${pathosg}/include \ 
               $${pathdragger}/include
				

CONFIG(debug, debug|release){
LIBS += -L../../bin -lMapFrameworkd
} else {
LIBS += -L../../bin -lMapFramework
}
