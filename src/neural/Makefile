# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS = -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0` -lm

SRC = $(filter-out no-make/*, $(wildcard *.c))
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC_TEST = no-make/test.c
OBJ_TEST = ${SRC_TEST:.c=.o}
DEP_TEST = ${SRC_TEST:.c=.d}


train: ${OBJ}

no-make/test: ${OBJ} ${OBJ_TEST}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_TEST}
	${RM} ${DEP} ${DEP_TEST}
	${RM} train no-make/test

# END
