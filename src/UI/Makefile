# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -g -O2 `pkg-config --cflags gtk+-3.0` 
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0`

SRC = main.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

ui: main.o $(OBJ)

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} UI

# END
