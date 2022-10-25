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

int comp_el_value(SDL_Surface* surface, int i, int j, const Kernel* kernel)
{
    int radius = kernel->radius;
    int* matrix = kernel->matrix;

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
    
    return value; 
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

    Kernel kernel = { .radius = radius, .matrix = mat};


    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* npixels = temp-> pixels;

    int value;
    for (int i = 0;i < surface-> h; i++)
    {
        for ( int j = 0; j < surface -> w; j++)
        {
            value = comp_el_value(surface,i,j,&kernel) / full_w;

            npixels[i* surface->w + j] = SDL_MapRGB(surface->format, value,value,value);  
        }
    }

     
    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}

SDL_Surface* SobelTransform(SDL_Surface* surface)
{
    //we use a 5x5 kernel here, may not be needed or may need 7x7
    int matx[25] = 
    {
        -5, -4, 0, 4, 5, 
        -8, -10, 0, 10, 8,
        -10, -20, 0, 20, 10,
        -8, -10, 0, 10, 8,
        -5, -4, 0, 4, 5
    };

    int maty[25] = 
    {
        -5,-8,-10,-8,-5,
        -4,-10,-20,-10,-4,
        0,0,0,0,0,
        4,10,20,10,4,
        5,8,10,8,5
    };

    Kernel Gx = { .radius = 5, .matrix = matx};

    Kernel Gy = { .radius = 5, .matrix = maty};
    
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* npixels = temp-> pixels;
    SDL_PixelFormat* format = surface->format;

    int valuex;
    int valuey;

    //convolution
    for (int i = 0;i < surface-> h; i++)
    {
        for ( int j = 0; j < surface -> w; j++)
        {
            valuex = comp_el_value(surface,i,j,&Gx);

            if (valuex != 0)
            {
                npixels[i* surface->w + j] = SDL_MapRGB(format,255,255,255); 
                continue;
            }
            
            valuey = comp_el_value(surface,i,j,&Gy);
        
            if (valuey != 0)
            {
                npixels[i* surface->w + j] = SDL_MapRGB(format,255,255,255); 
                continue;
            }
            
            npixels[i* surface->w + j] = SDL_MapRGB(format,0,0,0); 
        }
    }
     
    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}
