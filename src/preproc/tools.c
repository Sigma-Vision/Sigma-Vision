#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "struct.h"
#include "neutralize.h"

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

Uint8 GetColor_x(SDL_Surface* surface, int x)
{
    Uint8 r,g,b;

    Uint32* pixels = surface->pixels;

    SDL_GetRGB(pixels[x] , surface->format,&r,&g,&b);

    return r;
}

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

    }

    SDL_UnlockSurface(res);

    return res;
}



void GridSplit(SDL_Surface* surface)
{
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

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
            s.bottomLeft.X = h9*(i+1);

            SDL_Surface* temp = GridCropping(surface,&s);

            snprintf(filename,sizeof(filename),"r%i_c%i.case",i,j);
            IMG_SaveJPG(temp, filename,100);

            SDL_FreeSurface(temp);
        }
    }
}

//tester sans normalisation histogramme otsu
int case_white(SDL_Surface* surface)
{
    int var = OtsuGetMaxVariance(surface);    

    return var > 127;
}

int** ParseOutput()
{
    int** res = malloc(sizeof(int*)*81);

    int case_size = 28*28;
    char filename[15];

    for (int i = 0;i < 9;i++)
    {
        for (int j = 0;j < 9;j++)
        {
            snprintf(filename,sizeof(filename),"r%i_c%i.case",i,j);
            SDL_Surface* surface = load_image(filename);
            Uint32* pixels = surface->pixels;        

            int* arr = malloc(sizeof(int)*case_size);

            if (! case_white(surface))
            {
                for (int k = 0;k < case_size;k++)
                {
                     arr[k] = GetColor_x(surface,k);
                }
            }
            else
            {
                for (int k = 0;k < case_size;k++)
                {
                    arr[k] = SDL_MapRGB(surface->format,255,255,255);
                }
            }

            res[i*9+j] = arr; 
        }
    }

    return res;
}



SDL_Surface* ResizeSurface (SDL_Surface* surface,int n_width,int n_height)
{
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = surface->pixels;

    SDL_Surface* res = SDL_CreateRGBSurface(0,n_width,n_height,32,0,0,0,0);
    Uint32* respixels = res->pixels;
    
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    if (SDL_LockSurface(res) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
   
    GlobalMean* arr = calloc(n_width*n_height,sizeof(GlobalMean));

    for (int i = 0; i < height;i++)
    {
        for (int j = 0; < width;j++)
        {
            //get future coords
            //x = future i avec 0 <= x < n_height
            //y = future j avec 0 <= y < n_width
            //arr[x*n_width+y].value += GetColor(surface,i,j);
            //arr[x*n_width+y].num++;
        }
    }

    //in case of non set value at a certain coordinates, set default values for it 
    //uncomment to set default as white
    //current default : black
    //
    /*for (int i = 0;i < n_height;i++)
    {
        for (int j = 0;j < n_width;j++)
        {
            respixels[i*n_width+j] = SDL_MapRGB(surface->format,255,255,255);
        }
    }*/

    for (int i = 0;i < n_height;i++)
    {
        for (int j = 0;j < n_width;j++)
        {
            if (arr[i*n_width+j].value / arr[i*n_width+j].sum > 127)
                respixels[i*n_width+j] = SDL_MapRGB(surface->format,255,255,255);
            else
               respixes[i*n_width+j] = 0; 
        }
    }
    
    free(arr);

    SDL_UnlockSurface(res);

    SDL_FreeSurface(surface);

    return res;
}
