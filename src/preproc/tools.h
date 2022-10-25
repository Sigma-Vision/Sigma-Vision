#include <SDL2/SDL_image.h>

#ifndef TOOLS_H
#define TOOLS_H

SDL_Surface* load_image(const char* path);
SDL_Surface* GaussianBlur (SDL_Surface* surface, int radius);

#endif
