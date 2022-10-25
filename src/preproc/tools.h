#include <SDL2/SDL_image.h>

#ifndef TOOLS_H
#define TOOLS_H

Uint8 GetColor(SDL_Surface* surface, int i, int j);
SDL_Surface* load_image(const char* path);
SDL_Surface* GaussianBlur (SDL_Surface* surface, int radius);
SDL_Surface* SobelTransform(SDL_Surface* surface);

#endif
