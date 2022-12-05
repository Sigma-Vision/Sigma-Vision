# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -g -O2 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = 
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = no-make/main.c rebuild.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

rebuild: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
		${RM} ${OBJ}
		${RM} ${DEP}
		${RM} rebuild

# END
