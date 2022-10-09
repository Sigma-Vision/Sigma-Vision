#ifndef TOOLS_H
#define TOOLS_H

SDL_Surface* load_image(const char* path);
void rotate90ccw(SDL_Surface* surface);
void rotate90cw(SDL_Surface* surface);
#endif
