# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: test neutralize

SRC = test.c neutralize.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

test: test.o neutralize
neutralize: neutralize.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
