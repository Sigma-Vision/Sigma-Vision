#include <SDL2/SDL_image.h>
#include "struct.h"

#ifndef TOOLS_H
#define TOOLS_H

Uint8 GetColor(SDL_Surface* surface, int i, int j);
SDL_Surface* load_image(const char* path);
SDL_Surface* GridCropping(SDL_Surface* surface,Square* s);
void GridSplit(SDL_Surface* surface);

#endif
