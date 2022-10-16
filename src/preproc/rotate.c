#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * Rotates a surface 180 degrees in place
 *
 * surface: the surface to rotate
 */

void rotate180(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             temp[i*width + j] = surface[(height-i-1)*width+(width-j-1)]; 
        }
    }

    for(int i = 0; i < height*width;i++)
    {
        surface[i] = temp[i];
    }
    
    SDL_FreeSurface(temp);
}

/**
 * Rotates a surface 90 degrees counter clock wise in place
 *
 * surface: the surface to rotate
 */

void rotate90ccw(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);
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
    
    SDL_FreeSurface(temp);
}

/**
 * Rotates a surface 90 degrees clock wise in place
 *
 * surface: the surface to rotate
 */

void rotate90cw(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);
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

    SDL_FreeSurface(temp);
}

/**
 * WARNING : THIS FUNCTION MAY NOT WORK WELL ON NON SQUARE IMAGES.
 *
 * rotateAny : Rotates a surface by an arbitrary angle in place.
 * surface : the surface to rotate in place.
 * degree : the angle the surface will be rotated by.
 */

void rotateAny(SDL_Surface* surface,double angle)
{
    double width = surface->w;
    double height = surface->h;

    double center_x = height / 2;
    double center_y = width / 2;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    // check si 32 est bon
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    double newx;
    double newy;

    for (double i = 0;i < height;i++)
    {
        for (double j = 0;j < width;j++)
        {
            newx = (i-center_x) * cos(angle) - (j-center_y) * sin(angle) + center_x;
            
            if (newx < 0 || newx >= height)
               continue;
            
            newy = (i-center_x) * sin(angle) + (j-center_y) * cos(angle) + center_y;

             
            if (newy < 0 || newy >= height)
               continue;

            temp[(int)(newx * width + newy)] = surface[(int)(i*width+j)]; 
        }
    }

    
    for(int i = 0; i < height*width;i++)
    {
        surface[i] = temp[i];
    }

    SDL_FreeSurface(temp);

}
