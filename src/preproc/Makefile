# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -fsanitize=address -Wall -Wextra  `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SRC = no-make/main.c neutralize.c tools.c rotate.c scale.c transform.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

preproc: ${OBJ}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ} ${OBJ_TEST} preproc.o
	${RM} ${DEP} ${DEP_TEST} preproc.d
	${RM} preproc test
	${RM} r*_c*.case

# END
