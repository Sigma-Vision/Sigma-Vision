#ifndef NEUTR_H
#define NEUTR_H

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 binarize_pixel(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
#endif