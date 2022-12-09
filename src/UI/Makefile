# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -g -O2 `pkg-config --cflags gtk+-3.0` 
LDFLAGS =
LDLIBS = -lm `pkg-config --libs gtk+-3.0`

SRC = ui.c no-make/main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC_TEST = test.c
OBJ_TEST = ${SRC_TEST:.c=.o}
DEP_TEST = ${SRC_TEST:.c=.d}


ui: ${OBJ}

test: ${OBJ} ${OBJ_TEST}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_TEST}
	${RM} ${DEP} ${DEP_TEST}
	${RM} ui

# END
