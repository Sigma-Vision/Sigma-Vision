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

Uint8 GetColor(SDL_Surface* surface, int i, int j)
{
    Uint8 r,g,b;

    Uint32* pixels = surface->pixels;

    SDL_GetRGB(pixels[i*surface->w+j] , surface->format,&r,&g,&b);

    return r;
}

/*
Uint32 SetColor(int value)
{
    return SDL_MapRGB(format, value,value,value);
}
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

Uint32 comp_el_value(SDL_Surface* surface, int i, int j, const Kernel* kernel)
{
    int radius = kernel->radius;
    int* matrix = kernel->matrix;
    int full_w = kernel->full_kern_weight;

    int width = surface->w;
    int height = surface->h;

    int value = 0;

    int index_y;
    int index_x;
    
    for (int k = 0; k < radius*radius; k++)
    {
        index_y = i + (k/radius - radius/2);

        if (index_y < 0 || index_y >= height)
            continue;


        index_x = j + (k % radius - radius/2);

        if (index_x < 0 || index_x >= width)
            continue;

        value+= matrix[k] * GetColor(surface,index_y,index_x); 
    }
    
    value /= full_w; 
    return SDL_MapRGB(surface->format, value,value,value); 
}

SDL_Surface* convolution (SDL_Surface* surface, const Kernel* kernel)
{

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
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

SDL_Surface* GaussianBlur (SDL_Surface* surface,int radius)
{
    //test with several radiuses

    int* mat = calloc(radius*radius,sizeof(int));

    int full_w = 0;
    int mat_val = 1;

    for (int k = 0;k < radius*radius;k++)
    {
        *(mat+k) = mat_val;
        full_w += mat_val;
        
        if (k % radius == radius - 1)
        {
            if (k / radius < radius/2)
               mat_val *= 2;
            else
               mat_val /= 2; 
        }
        else
        {
            if (k % radius < radius/2)
                mat_val *= 2;
            else
                mat_val /= 2;
        }
    }

    Kernel kernel = { .radius = radius, .matrix = mat,
        .full_kern_weight = full_w};

    return convolution (surface,&kernel);
}

SDL_Surface* SobelTransform(SDL_Surface* surface, int radius)
{
    
}
