
SET(LIBRARY_NAME MapFramework)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIBRARY_NAME})

#SET(HEADERS
#		${HEADER_PATH}/BaseMainWindow.h 
#		${HEADER_PATH}/BaseSceneManager.h 
#		${HEADER_PATH}/CollectVisitor.h 
#		${HEADER_PATH}/CustomerDragger.h 
#		${HEADER_PATH}/CustomerPickHandler.h 
#		${HEADER_PATH}/CustomerSceneManager.h 
#		${HEADER_PATH}/CustomerSceneManipulator.h 
#		${HEADER_PATH}/EditLine.h 
#		${HEADER_PATH}/Export.h 
#		${HEADER_PATH}/MapEditor.h 
#		${HEADER_PATH}/MapShow.h  
#		${HEADER_PATH}/ObjectsRecord.h 
#		${HEADER_PATH}/PropertyWidget.h 
#		${HEADER_PATH}/ResourceManager.h 
#		${HEADER_PATH}/ToolList.h 
#		${HEADER_PATH}/TreeWidget.h 
#		${HEADER_PATH}/Utils.h 
#		${HEADER_PATH}/StateCollect.h 
#		${HEADER_PATH}/ImageResource.h
#	)

#SET(SOURCES 
#		library.cpp
#	)
	
AUX_SOURCE_DIRECTORY(. SOURCES)
FILE(GLOB HEADERS ${HEADER_PATH}/*.h )

SET( DEFINITION ${LIBRARY_NAME} )
STRING( TOUPPER ${LIBRARY_NAME} DEFINITION)
OPTION(DYNAMIC_${DEFINITION} "Set to ON to build  dynamic.  Use OFF for static." ON)

INCLUDE_DIRECTORIES(
					#${PROJECT_SOURCE_DIR}/include/${LIBRARY_NAME}
					${PROJECT_SOURCE_DIR}/include
					${CMAKE_CURRENT_BINARY_DIR}
					
					${OPENSCENEGRAPH_INCLUDE_DIR}
					${osgVirtualAssembly_INCLUDE_DIR}
					#database include
					${KDFDB_INCLUDE_DIR}
				   )
					
LINK_DIRECTORIES(
				${PROJECT_BINARY_DIR}/lib
				
				${OPENSCENEGRAPH_LIB_DIR}
				${osgVirtualAssembly_LIB_DIR}
				#database link
				${KDFDB_LIB_DIR}
				)

#SET(QT_USE_QTOPENGL TRUE)
#SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSQL TRUE)
SET(FORMS_UI)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
QT4_WRAP_CPP( HEADERS_MOC ${HEADERS} )
QT4_WRAP_UI( FORMS_HEADERS ${FORMS_UI} )

LIBRARY()

TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
	opengl32 
	glu32
	debug OpenThreadsd									optimized OpenThreads
    debug osgd       									optimized osg
	debug osgGAd	  									optimized osgGA
	debug osgTextd        								optimized osgText
	debug osgManipulatord    							optimized osgManipulator
	debug osgUtild        								optimized osgUtil
	debug osgDBd                                        optimized osgDB
	debug osgTextd										optimized osgText
	debug osgViewerd									optimized osgViewer
	debug osgQtd										optimized osgQt
	debug osgDraggerd									optimized osgDragger
	debug osgScened										optimized osgScene
	#debug Databased									optimized Database
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_QTOPENGL_LIBRARIES}
	${QT_QTXML_LIBRARIES}
	)