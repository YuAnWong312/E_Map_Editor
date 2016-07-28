FIND_PATH(
	KDFDB_INCLUDE_DIR kdfdb.h
	PATHS
	$ENV{KDFDB_ROOT}/include
	)
	
FIND_PATH(
	KDFDB_LIB_DIR kdfdb.lib libkdfdb.so libkdfdb.a
	PATHS
	$ENV{KDFDB_ROOT}/lib
	)
	
SET(KDFDB "NO")
IF(KDFDB_INCLUDE_DIR AND KDFDB_LIB_DIR)
SET(KDFDB "YES")
ENDIF(KDFDB_INCLUDE_DIR AND KDFDB_LIB_DIR)