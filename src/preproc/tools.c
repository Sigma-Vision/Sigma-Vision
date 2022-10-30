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
    
    free(mat);

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
    
    /*

    //radius = 3

    int matx[9] = 
    {
        -1,0,1,
        -2,0,2,
        -1,0,1
    };
    
    int maty[9] =
    {
        -1,-2,-1,
        0,0,0,
        -1,2,1
    };

    Kernel Gx = {.radius = 3, .matrix = matx};

    Kernel Gy = {.radius = 3, .matrix = maty};
    
    */

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* npixels = temp-> pixels;
    SDL_PixelFormat* format = surface->format;

    int valuex;
    int valuey;
    double G;
    int value;
    //double angle;

    //convolution
    for (int i = 0;i < surface-> h; i++)
    {
        for ( int j = 0; j < surface -> w; j++)
        {
            valuex = comp_el_value(surface,i,j,&Gx);
            valuey = comp_el_value(surface,i,j,&Gy);

            G = sqrt(valuex*valuex + valuey*valuey);

            //angle = atan((double)valuey / (double)valuex);

            //compute the G of (x+1,y+1) et (x-1,y-1) (déterminés selon l'angle)
            //determine if value = 0 or 255
            //
            //then put treshold in place
            //printf("%f\n",angle);
            value = G > 100 ? 255 : 0;
            npixels[i* surface->w + j] = SDL_MapRGB(format,value,value,value); 
        }
    }
     
    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}


/*SDL_Surface* CannyTransform(SDL_Surface* surface)
{
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    surface = SobelTransform(surface);
}*/

SDL_Surface* GridCropping (SDL_Surface* surface, Dot* dot1,Dot* dot2)
{
    int width = surface->w;
    //int height = surface->h;
    Uint32* pixels = surface->pixels;

    int side_len = dot2->X - dot1->X;

    SDL_Surface* res = SDL_CreateRGBSurface(0,side_len,side_len,32,0,0,0,0);
    Uint32* respixels = res->pixels;

    for (int i = 0; i < side_len; i++)
    {
        for (int j = 0 ; j < side_len; j++)
        {
            respixels[i*side_len+j] = pixels[(i+dot1->X)*width+(j+dot1->X)];
        }
    } 
    
    return res;
}



void GridSplit(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;

    int w9 = width/9;
    int h9 = height/9;
    
    char filename[30];

    for (int i = 0;i < 9;i++)
    {
        for (int j = 0;j < 9;j++)
        {
            Dot dot1;
            dot1.X = w9*j;
            dot1.Y = h9*i;

            Dot dot2;
            dot2.X = w9*(j+1);
            dot2.Y = h9*(i+1);

            SDL_Surface* temp = GridCropping(surface,&dot1,&dot2);
            
            snprintf(filename,sizeof(filename),"case_r%c_c%c",i+48,j+48);
            IMG_SaveJPG(temp, filename,100);

            SDL_FreeSurface(temp);
        }
    }
}
