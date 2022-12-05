#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "struct.h"

/** 
 * Loads an image in a surface.
 * The format of the surface is SDL_PIXELFORMAT_RGB888.
 *
 * path: Path of the image.
 */


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


SDL_Surface* GridCropping (SDL_Surface* surface, Square* s)
{
    /**
     * WARNING : THIS FUNCTION DOES NOT FREE THE SURFACE surface, 
     * BUT STILL RETURNS ANOTHER SURFACE. PLEASE FREE THE SUPPLIED 
     * SURFACE IF YOU DO NOT NEED IT AFTERWARDS
     */
    
    // X = vertical | Y = horizontal


    int width = surface->w;
    Uint32* pixels = surface->pixels;

    int side_len_y = s->topRight.Y - s->topLeft.Y;
    int side_len_x =  s->bottomLeft.X - s->topLeft.X ;

    Dot dot1 = s->topLeft; 

    printf("side len : x = %i _ y = %i | img width = %i\n",side_len_x,side_len_y,surface->h); 

    SDL_Surface* res = SDL_CreateRGBSurface(0,side_len_y,side_len_x,32,0,0,0,0);
    Uint32* respixels = res->pixels;
    
    if (SDL_LockSurface(res) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < side_len_x; i++)
    {
        for (int j = 0 ; j < side_len_y; j++)
        {
            respixels[i*side_len_y+j] = pixels[(i+dot1.X)*width+(j+dot1.Y)];
        }

        //if (i > 1000)
            printf("DOT3 : X = %i and Y = %i\n",s->bottomLeft.X, s->bottomLeft.Y);
            //errx(1,"DOT1 : X = %i and Y = %i | DOT2 : X = %i and Y = %i\n",dot1.X,dot1.Y,s->topRight.X,s->topRight.Y);    
    }

    SDL_UnlockSurface(res);

    return res;
}



void GridSplit(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;

    int w9 = width/9;
    int h9 = height/9;
    
    char filename[15];

    Square s;
    
    for (int i = 0;i < 9;i++)
    {
        for (int j = 0;j < 9;j++)
        {
            s.topLeft.Y = w9*j;
            s.topLeft.X = h9*i;

            s.topRight.Y = w9*(j+1);
            s.topRight.X = h9*i;

            s.bottomLeft.Y = w9*j;
            s.bottomLeft.X = h9*(j+1);

            SDL_Surface* temp = GridCropping(surface,&s);

            snprintf(filename,sizeof(filename),"r%i_c%i.case",i,j);
            IMG_SaveJPG(temp, filename,100);

            SDL_FreeSurface(temp);
        }
    }
}
