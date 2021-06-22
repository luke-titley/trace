SRC=png.c\
pngerror.c\
pnggccrd.c\
pngget.c\
pngmem.c\
pngpread.c\
pngread.c\
pngrio.c\
pngrtran.c\
pngrutil.c\
pngset.c\
pngtrans.c\
pngvcrd.c\
pngwio.c\
pngwrite.c\
pngwtran.c\
pngwutil.c

OBJ=png.o\
pngerror.o\
pnggccrd.o\
pngget.o\
pngmem.o\
pngpread.o\
pngread.o\
pngrio.o\
pngrtran.o\
pngrutil.o\
pngset.o\
pngtrans.o\
pngvcrd.o\
pngwio.o\
pngwrite.o\
pngwtran.o\
pngwutil.o

libpng.so: ${SRC}
	gcc -I./ -fPIC -c ${SRC}
	gcc -shared -o ../lib/libpng.so ${OBJ}

all: libpng.so
