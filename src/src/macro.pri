
# target_name is the app name
TARGET = $$target_name

# program_type is the app type
TEMPLATE = $$program_type

# include_path is the INCLUDEPATH
INCLUDEPATH *= $$include_path

# destdir is the DESTDIR
isEmpty($$destdir){
	destdir = ../../bin/
}
DESTDIR = $$destdir

CONFIG(debug, debug|release){
     CONFIG += console
     TARGET = $${TARGET}d
} else {
     TARGET = $$TARGET
}


