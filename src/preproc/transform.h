#include <SDL2/SDL_image.h>
#include "struct.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

SDL_Surface* GaussianBlur (SDL_Surface* surface, int radius);
SDL_Surface* SobelTransform(SDL_Surface* surface,int aggro);

#endif
