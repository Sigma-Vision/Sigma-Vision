#include <SDL2/SDL_image.h>
#include "struct.h"

#ifndef ROTATE_H
#define ROTATE_H

SDL_Surface* rotateAny(SDL_Surface* surface, double angle,int color_fill);
void rotate180(SDL_Surface* surface);
SDL_Surface* rotate90ccw(SDL_Surface* surface);
SDL_Surface* rotate90cw(SDL_Surface* surface);
SDL_Surface* RotateDetectedGrid(SDL_Surface* surface,Dot* dot1, Dot* dot2);

#endif
