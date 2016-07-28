::set OSG_ROOT = "D:/osg-3.1.2"
::set OSGDRAGGER_ROOT = "C:/Program Files/osgVirtualAssembly"


::OSG_ROOT = "D:/osg-3.1.2"
::OSGDRAGGER_ROOT = "C:/Program Files/osgVirtualAssembly"
::cmake ../src -L -G "Visual Studio 9 2008" -DOPENSCENEGRAPH_INCLUDE_DIR=@OSG_ROOT/include -DOPENSCENEGRAPH_LIB_DIR=@OSG_ROOT/lib -DosgVirtualAssembly_INCLUDE_DIR=@OSGDRAGGER_ROOT/include -DosgVirtualAssembly_LIB_DIR=@OSGDRAGGER_ROOT/lib -DQT_QMAKE_EXECUTABLE=C:\Qt\4.8.1\bin\qmake
::mkdir bin
::cd bin
::cmake .. 
-L -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE=Debug -DOPENSCENEGRAPH_INCLUDE_DIR=D:/osg-3.1.2/include -DOPENSCENEGRAPH_LIB_DIR=D:/osg-3.1.2/lib -DosgVirtualAssembly_INCLUDE_DIR="C:\Program Files\osgVirtualAssembly\include" -DosgVirtualAssembly_LIB_DIR="C:\Program Files\osgVirtualAssembly\lib" -DKDFDB_INCLUDE_DIR="E:\qtcreator_cmake_12-29\src\KDFDB\include" -DKDFDB_LIB_DIR="E:\qtcreator_cmake_12-29\src\KDFDB\lib" -DQT_QMAKE_EXECUTABLE=C:/Qt/4.8.1/bin/qmake

-L -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DOPENSCENEGRAPH_INCLUDE_DIR="C:/Program Files/OpenSceneGraph_MinGW/include" -DOPENSCENEGRAPH_LIB_DIR="C:/Program Files/OpenSceneGraph_MinGW/lib" -DosgVirtualAssembly_INCLUDE_DIR="C:/Program Files/osgVirtualAssembly_MinGW/include" -DosgVirtualAssembly_LIB_DIR="C:/Program Files/osgVirtualAssembly_MinGW/lib" -DKDFDB_INCLUDE_DIR="F:/KDFDB/include" -DKDFDB_LIB_DIR="F:/KDFDB/lib" -DQT_QMAKE_EXECUTABLE=D:/Qt/2010.05/qt/bin/qmake