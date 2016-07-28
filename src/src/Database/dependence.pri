
include(../path.pri)
INCLUDEPATH += $$pathKDFDB/include

CONFIG(debug, debug|release){
    LIBS +=  $$pathKDFDB/lib/kdfdbd.dll
    LIBS +=  $$pathKDFDB/lib/libmysql.dll
} else {
    LIBS +=  $$pathKDFDB/lib/kdfdb.dll
    LIBS +=  $$pathKDFDB/lib/libmysql.dll
}
