#include <stdio.h>
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
    Uint32* pixels = surface -> pixels;
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    Uint32* npixels = temp -> pixels;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             npixels[i*width + j] = pixels[(height-i-1)*width+(width-j-1)]; 
        }
    }

    for(int i = 0; i < height*width;i++)
    {
        pixels[i] = npixels[i];
    }
    
    SDL_UnlockSurface(surface);

    SDL_FreeSurface(temp);
}

/**
 * Rotates a surface 90 degrees counter clock wise 
 *
 * surface: the surface to rotate
 *
 * returns : the rotated surface
 */

SDL_Surface* rotate90ccw(SDL_Surface* surface)
{
    Uint32* pixels = surface -> pixels;
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);
    Uint32* npixels = temp-> pixels;

    // check si 32 est bon
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            npixels[(width-j-1)*width + i] = pixels[i*width+j]; 
        }
    }

    SDL_FreeSurface(surface);
    
    SDL_UnlockSurface(temp);

    return temp;
}

/**
 * Rotates a surface 90 degrees clock wise
 *
 * surface: the surface to rotate
 * 
 * returns : the rotated surface
 */

SDL_Surface* rotate90cw(SDL_Surface* surface)
{
    Uint32* pixels = surface -> pixels;
    int width = surface->w;
    int height = surface->h;

    SDL_Surface* temp = SDL_CreateRGBSurface(0,height,width,32,0,0,0,0);
    Uint32* npixels = temp-> pixels;
    // check si 32 est bon
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             npixels[j*width + (height - i - 1)] = pixels[i*width+j]; 
        }
    }

    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}

void get_max_coord(SDL_Surface* surface, double angle, int* maxx, int* minx,int* maxy, int* miny)
{

    double center_x = surface->w / 2;
    double center_y = surface->h / 2;

    double nmaxx = surface->w; 
    double nminx = 0;
    double nmaxy = surface->h;
    double nminy = 0;

    double newx;
    double newy;

    for (double i = 0;i < surface->h;i++)
    {
        for (double j = 0;j < surface->w;j++)
        {
            newx = (j-center_x) * cos(angle) - (i-center_y) * sin(angle) + center_x;
            
            if (newx < nminx) 
               nminx = newx;
            if (newx > nmaxx)
                nmaxx = newx;
            
            newy = (j-center_x) * sin(angle) + (i-center_y) * cos(angle) + center_y;
             
            if (newy < nminy)
               nminy = newy;

            if (newy > nmaxy)
                nmaxy = newy;
        }
    }
    
    *maxx = (int) nmaxx;
    *minx  = (int) nminx; 
    *maxy = (int) nmaxy;
    *miny = (int) nminy;
}

/**
 *
 * rotateAny : Rotates a surface by an arbitrary angle in place.
 * surface : the surface to rotate in place.
 * angle : the angle the surface will be rotated by.
 *
 * returns : the rotated surface
 */

SDL_Surface* rotateAny(SDL_Surface* surface,double angle)
{
    //width = x
    //height = y

    Uint32* pixels = surface -> pixels;
    double center_x = surface->w / 2;
    double center_y = surface->h / 2;

    int maxx;
    int minx;
    int maxy;
    int miny;
    
    get_max_coord(surface,angle,&maxx,&minx,&maxy,&miny);
    // this function gets the desired width and height for the rotated surface
    // in order to have every point in the surface 

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    //SDL_Surface* temp = SDL_CreateRGBSurface(0,(maxx - minx),(maxy - miny),32,0,0,0,0);
    // check si 32 est bon

    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    Uint32* npixels = temp->pixels;

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    double newx;
    double newy;

    FILE* f = fopen("debug.txt","w");

    fprintf(f,"WIDTH = %i\nHEIGHT = %i\n",surface->w,surface->h);

    for (double i = 0;i < surface->h;i++)
    {
        for (double j = 0;j < surface->w;j++)
        {
            newx = (j-center_x) * cos(angle) - (i-center_y) * sin(angle) + center_x;
            
            newy = (j-center_x) * sin(angle) + (i-center_y) * cos(angle) + center_y;

            if (newx <= 0 || newx >= surface->w || newy <= 0 || newy >= surface->h)
                continue;

            // area mapping : use the first decimal of the double in order to
            // ponderate x,y ; x+1,y ; x,y+1 ; x+1,y+1.
            
            //currently debugging
            Uint8 r, g, b;

            SDL_GetRGB(pixels[(int)(i* surface->w +j)], surface->format, &r, &g, &b);
 
            fprintf(f,"[DEBUG] :\n NEWX = %f  | NEWY = %f  | OLDY = %f | OLDX = %f | Index = %i  | pixels[i] = %u\n", newx, newy, i, j, (int) (i*surface->w + j), r);
            
            npixels[(int)(newy * surface->w + newx)] = pixels[(int)(i* surface->w +j)]; 
        }
    }

    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}
