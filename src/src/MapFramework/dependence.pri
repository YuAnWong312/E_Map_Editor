
include(../path.pri)
INCLUDEPATH += 	$$pathosg/include \
				$$pathdragger/include \
				$$pathdatabase/include \
				../../include/Database
			   
#DEPENDPATH +=	$$PWD/C:/Program Files/OpenSceneGraph/include \ 
#				$$PWD/C:/Program Files/osgVirtualAssembly/include

CONFIG(debug, debug|release){
    LIBS += -L$$pathosg/lib      -lOpenThreadsd -losgd -losgGAd -losgManipulatord -losgUtild -losgViewerd -losgQtd -losgDBd -losgTextd \
            -L$$pathdragger/lib  -losgDraggerd

    #database
    LIBS += -L../../bin          -lDatabased
}else{
    LIBS += -L$$pathosg/lib      -lOpenThreads -losg -losgGA -losgManipulator -losgUtil -losgViewer -losgQt -losgDB -losgText \
            -L$$pathdragger/lib  -losgDragger

    #database
    LIBS += -L../../bin          -lDatabase
}
