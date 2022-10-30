# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -fsanitize=address -Wall -Wextra  `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = no-make/main.c neutralize.c tools.c rotate.c scale.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

preproc: ${OBJ}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_TEST}
	${RM} ${DEP} ${DEP_TEST}
	${RM} preproc test

# END
