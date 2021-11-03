
GXX=g++
STRIP=strip
RM=rm -rf

SOURCE = \
main.cpp \
source/txh_SDL2.cpp \
source/txh_FFmpeg.cpp

TARGET = main
INDCLUDE_DIR = include
LIB_DIR = lib

FLAGS = \
-lSDL2 -lm -ldl -lpthread -lrt \
-lavdevice -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil \
-lz -llzma -lm -lbz2 -lva

# 检测lib目录，不否在则解压lib.tar.gz
test_lib_dir = $(shell if [ ! -d $(LIB_DIR) ]; then echo "tar -zxvf lib.tar.gz"; tar -zxvf lib.tar.gz; fi)
$(info $(test_lib_dir))

all:
	${GXX} ${SOURCE}  -I ${INDCLUDE_DIR} -L ${LIB_DIR} ${FLAGS} -o ${TARGET}
	${STRIP} ${TARGET}

clean:
	${RM} -rf ${TARGET} *.o
