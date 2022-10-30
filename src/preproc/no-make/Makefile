# Makefile

CC = gcc 
CPPFLAGS =
CFLAGS = -fsanitize=address -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: neutralize test 

SRC = ../neutralize.c ../tools.c ../rotate.c ../scale.c test.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

test: ../rotate.o test.o ../neutralize.o ../tools.o ../rotate.h \
	../neutralize.h ../tools.h ../scale.h ../scale.o
neutralize: ../neutralize.o ../tools.o ../rotate.o ../rotate.h \
	../neutralize.h ../tools.h ../scale.h ../scale.o

rotate: ../rotate.h ../rotate.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
