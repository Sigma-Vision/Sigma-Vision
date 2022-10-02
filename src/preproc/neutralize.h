#ifndef NEUTR_H
#define NEUTR_H

SDL_Surface* load_image(const char* path);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
#endif
