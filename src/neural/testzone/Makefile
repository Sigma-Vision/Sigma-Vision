# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS = -lm        # link math lib

SRC = $(wildcard *.c)
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

xor: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} xor

# END
