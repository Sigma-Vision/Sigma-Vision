#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <math.h>
#include <err.h>

#include "struct.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#ifndef SCALE_H
#define SCALE_H

void find_grid(SDL_Surface* surface, Square* s);
void find_coin(SDL_Surface* surface, Square* s);
#endif
