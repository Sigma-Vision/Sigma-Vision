#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/** 
 * Loads an image in a surface.
 * The format of the surface is SDL_PIXELFORMAT_RGB888.
 *
 * path: Path of the image.
 */

typedef struct Kernel
{
    int radius;
    int* matrix;
    int full_kern_weight;
}Kernel;


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

Uint32 comp_el_value(SDL_Surface* surface, int i, int j, Kernel kernel)
{
    int radius = kernel->radius;
    int* matrix = kernel->matrix;
    int full_w = kernel->full_kern_weight;

    Uint32* pixels = surface-> pixels;
    int width = surface->w;
    int height = surface->h;

    int value = 0;
    
    for (int k = 0; k < radius*radius; k++)
    {
        index_y = i + (k/3 - radius/2);

        if (index_y < 0 || index_y >= height)
            continue;


        index_x = j + (k % 3 - radius/2);

        if (index_x < 0 || index_y >= height)
            continue;

        value+= matrix[k] * pixels[index_y * width + index_y]; 
    }
            
    return value; 
}

SDL_Surface* GaussianBlur(SDL_Surface* surface)
{
    //maybe not get a fixed kernel or with a higher radius 
    Kernel kernel = { .radius = 3, .matrix = { 1, 2, 1, 2, 4, 2, 1, 2, 1},
        .full_kern_weight = 16}; 

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    Uint32* pixels = surface -> pixels; 
   
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* npixels = temp-> pixels;


    for (int i = 0;i < surface-> h; i++)
    {
        for ( int j = 0; j < surface -> w; j++)
        {
            npixels[i* surface->w + j] = comp_el_value(surface,i,j,kernel);
        }
    }

     
    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}
