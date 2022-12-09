#include <SDL2/SDL_image.h>
#include "struct.h"

#ifndef TOOLS_H
#define TOOLS_H

Uint8 GetColor(SDL_Surface* surface, int i, int j);
SDL_Surface* load_image(const char* path);
SDL_Surface* GridCropping(SDL_Surface* surface,Square* s);
void GridSplit(SDL_Surface* surface);
SDL_Surface* ResizeSurface(SDL_Surface* surface,int n_width,int n_height);
void print_pixel(SDL_Surface* surface, int i, int j, Uint8 r, Uint8 g, Uint8 b);
void fill_outside_square(SDL_Surface* surface, Square* s);
void print_square(SDL_Surface* surface, Square* s);

#endif
