#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/** 
 * Loads an image in a surface.
 * The format of the surface is SDL_PIXELFORMAT_RGB888.
 *
 * path: Path of the image.
 */

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* res = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
    
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_FreeSurface(surface);

    return res;
}

/**
 * Rotates a surface 90 degrees counter clock wise in place
 *
 * surface: the surface to rotate
 */

void rotate90ccw(SDL_Surface* surface)
{
    int width,height = surface->w, surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    // check si 32 est bon
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             temp[(width-j-1)*width + i] = surface[i*width+j]; 
        }
    }

    for(int i = 0; i < height*width;i++)
    {
        surface[i] = temp[i];
    }
}

/**
 * Rotates a surface 90 degrees clock wise in place
 *
 * surface: the surface to rotate
 */

void rotate90cw(SDL_Surface* surface)
{
    int width,height = surface->w, surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    // check si 32 est bon
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             temp[j*width + (height - i - 1)] = surface[i*width+j]; 
        }
    }

    for(int i = 0; i < height*width;i++)
    {
        surface[i] = temp[i];
    }
}
