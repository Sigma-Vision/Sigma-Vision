#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "struct.h"
#include "tools.h"

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

void fill_matrices_sobel(int* matx, int* maty, int aggro)
{
    if (aggro == 5)
    {
        matx[0] = -5;
        matx[1] = -4;
        matx[2] = 0;
        matx[3] = 4;
        matx[4] = 5;
        matx[5] = -8;
        matx[6] = -10;
        matx[7] = 0;
        matx[8] = 10;
        matx[9] = 8;
        matx[10] = -10;
        matx[11] = -20;
        matx[12] = 0;
        matx[13] = 20;
        matx[14] = 10;
        matx[15] = -8;
        matx[16] = -10;
        matx[17] = 0;
        matx[18] = 10;
        matx[19] = 8;
        matx[20] = -5;
        matx[21] = -4;
        matx[22] = 0;
        matx[23] = 4;
        matx[24] = 5;
        
        maty[0] = -5;
        maty[1] = -8;
        maty[2] = -10;
        maty[3] = -8;
        maty[4] = -5;
        maty[5] = -4;
        maty[6] = -10;
        maty[7] = -20;
        maty[8] = -10;
        maty[9] = -4;
        maty[10] = 0;
        maty[11] = 0;
        maty[12] = 0;
        maty[13] = 0;
        maty[14] = 0;
        maty[15] = 4;
        maty[16] = 10;
        maty[17] = 20;
        maty[18] = 10;
        maty[19] = 4;
        maty[20] = 5;
        maty[21] = 8;
        maty[22] = 10;
        maty[23] = 8;
        maty[24] = 5;

    }

    if (aggro == 3)
    {
        matx[0] = -1; 
        matx[1] =  0;
        matx[2] =  1;
        matx[3] = -2;
        matx[4] =  0;
        matx[5] =  2;
        matx[6] = -1; 
        matx[7] =  0;
        matx[8] =  1;

        maty[0] = -1; 
        maty[1] = -2; 
        maty[2] = -1; 
        maty[3] =  0;
        maty[4] =  0;
        maty[5] =  0;
        maty[6] =  1;
        maty[7] =  2;
        maty[8] =  1;

    }
}

/**
 * THIS FUNCTIONS DOES NOT FREE THE PROVIDED SURFACE AND RETURNES ANOTHER
 * WATCH OUT FOR USE AFTER FREE
 */
SDL_Surface* SobelTransform(SDL_Surface* surface,int aggro)
{
    //we use a 5x5 kernel here, may not be needed or may need 7x7
     
    Kernel Gx;
    Kernel Gy;

    if (aggro == 5)
    {
        int* matx = malloc(25*sizeof(int));
        int* maty = malloc(25*sizeof(int));
        
        fill_matrices_sobel(matx, maty,aggro);
        
        Gx.radius = 5;
        Gx.matrix = matx;

        Gy.radius = 5;
        Gy.matrix = maty;
    }
    else
    {
        //radius = 3
        int* matx = malloc(9*sizeof(int));
        int* maty = malloc(9*sizeof(int));

        fill_matrices_sobel(matx, maty,aggro);

        Gx.radius = 3;
        Gx.matrix = matx;
        
        Gy.radius = 3;
        Gy.matrix = maty;
    }
    

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
     
    //SDL_FreeSurface(surface);
    //
    free(Gy.matrix);
    free(Gx.matrix);

    SDL_UnlockSurface(temp);

    return temp;
}


SDL_Surface* CannyTransform(SDL_Surface* surface)
{
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    surface = SobelTransform(surface,3);

    SDL_UnlockSurface(surface);

    return surface;
}

