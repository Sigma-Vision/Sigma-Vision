#include <SDL2/SDL_image.h>
#include "struct.h"

#ifndef TOOLS_H
#define TOOLS_H

Uint8 GetColor(SDL_Surface* surface, int i, int j);
SDL_Surface* load_image(const char* path);
SDL_Surface* GaussianBlur (SDL_Surface* surface, int radius);
SDL_Surface* SobelTransform(SDL_Surface* surface);
SDL_Surface* GridCropping(SDL_Surface* surface,Dot* dot1, Dot* dot2);
void GridSplit(SDL_Surface* surface);

#endif
