# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra 
LDFLAGS =
LDLIBS = -lm

SRC = no-make/main.c solver.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC_TEST = test.c
OBJ_TEST = ${SRC_TEST:.c=.o}
DEP_TEST = ${SRC_TEST:.c=.d}


solver: ${OBJ}

test: ${OBJ} ${OBJ_TEST}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_TEST}
	${RM} ${DEP} ${DEP_TEST}
	${RM} solver test

# END
