FIND_PATH(osgVirtualAssembly_INCLUDE_DIR osgDragger/DraggerManager
  PATHS
  $ENV{OSGDRAGGER_ROOT}/include
  $ENV{OSGDRAGGER_ROOT}/include
  $ENV{OSGDRAGGER_ROOT}/include
  /usr/include
  /usr/local/include
)

FIND_PATH(osgVirtualAssembly_LIB_DIR osgDragger.lib libosgDragger.so
  PATHS
  $ENV{OSGDRAGGER_ROOT}/lib
  $ENV{OSGDRAGGER_ROOT}/lib
  $ENV{OSGDRAGGER_ROOT}/lib
  /usr/lib 
  /usr/local/lib
)

SET(OSGDRAGGER_FOUND "NO")

IF(osgVirtualAssembly_INCLUDE_DIR AND osgVirtualAssembly_LIB_DIR)
SET(OSGDRAGGER_FOUND "YES")
ENDIF(osgVirtualAssembly_INCLUDE_DIR AND osgVirtualAssembly_LIB_DIR)
