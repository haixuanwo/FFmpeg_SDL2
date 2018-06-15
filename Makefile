
SOURCE = \
main.cpp \
source/txh_SDL2.cpp \
source/txh_FFmpeg.cpp

TARGET = main

INDCLUDE_DIR = include

LIB_DIR = lib

FLAG = \
-lSDL2 -lm -ldl -lpthread -lrt \
-lavdevice -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil \
-lz -llzma -lm -lbz2 -lva

#FLAG = -lSDL2 -lSDL2main -ldl -lrt \
#-lavdevice -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil \
#-lpthread -lz -lm

all:
	g++ ${SOURCE}  -I ${INDCLUDE_DIR} -L ${LIB_DIR} ${FLAG} -o ${TARGET}
clean:
	rm -rf ${TARGET} *.o
	
