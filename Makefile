# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = $(filter-out src/test.c, $(wildcard src/*.c))
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC_CHECK = src/test.c
OBJ_CHECK = ${SRC_CHECK:.c=.o}
DEP_CHECK = ${SRC_CHECK:.c=.d}


main: ${OBJ}

check: ${OBJ} ${OBJ_CHECK}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_CHECK}
	${RM} ${DEP} ${DEP_CHECK}
	${RM} main check

# END
