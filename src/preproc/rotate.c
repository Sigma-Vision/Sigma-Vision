#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tools.h"

#define PI 3.14159265

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

    int nmaxx = surface->w; 
    int nminx = 0;
    int nmaxy = surface->h;
    int nminy = 0;

    int newx;
    int newy;

    for (double y = 0;y < surface->h;y++)
    {
        for (double x = 0;x < surface->w;x++)
        {
            newx = (x-center_x) * cos(angle) - (y-center_y) * sin(angle) + center_x;
            
            if (newx < nminx) 
               nminx = newx;
            
            if (newx > nmaxx)
                nmaxx = newx;
            
            newy = (x-center_x) * sin(angle) + (y-center_y) * cos(angle) + center_y;
            if (newy < nminy)
               nminy = newy;
            if (newy > nmaxy)
                nmaxy = newy;
        }
    }
    
    *maxx = nmaxx;
    *minx = nminx; 
    *maxy = nmaxy;
    *miny = nminy;
}

/**
 *
 * rotateAny : Rotates a surface by an arbitrary angle (in degrees).
 * surface : the surface to rotate.
 * angle : the angle the surface will be rotated by.
 * color_fill : the color for the "lost"pixels to be filled with. 
 * 0 if black and 255 if white
 *
 * returns : the rotated surface
 */

SDL_Surface* rotateAny(SDL_Surface* surface,double angle,char* color_fill)
{
    //width = x
    //height = y

    angle = angle*PI/180;
    // Angle from degrees to radian

    Uint32* pixels = surface -> pixels;

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

    //printf("MAXX = %i | MINX = %i | MAXY = %i | MINY = %i\n",maxx,minx,maxy,miny);

    //printf("temp->w = %i | temp->h = %i\n",temp->w,temp->h);

    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    double center_x = temp->w / 2;
    double center_y = temp->h / 2;
    
    Uint32* npixels = temp->pixels;

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32 color;

    switch (color_fill)
    {
        case "blue":
            color = SDL_MapRGB( temp->format, 0,0,255);
            break;
        case "green":
            color = SDL_MapRGB( temp->format, 0, 255, 0);
            break;
        case "red":
            color = SDL_MapRGB( temp->format, 255, 0, 0);
            break;
        case "black":
            color = SDL_MapRGB( temp->format, 0, 0, 0);
            break;
        case "white":
            color = SDL_MapRGB( temp->format, 255, 255, 255);
            break;
        default:
            // default case : color = blue
            color = SDL_MapRGB( temp->format, 0,0,255);
            break;
    }

    int srcx;
    int srcy;

    //FILE* f = fopen("debug.txt","w");
    //fprintf(f,"WIDTH = %i\nHEIGHT = %i\n",temp->w,temp->h);

    for (double y = 0;y < temp->h;y++)
    {
        for (double x = 0;x < temp->w;x++)
        {
            srcx = (x-center_x) * cos(angle) + (y-center_y) * sin(angle) + center_x;
            
            srcy = -(x-center_x) * sin(angle) + (y-center_y) * cos(angle) + center_y;
            

            if (srcx <= 0 || srcx >= surface->w || srcy <= 0 || srcy >= surface->h)
            {
                npixels[(int) y * temp->w + (int)x] = color; 
            } 
            else
                npixels[(int)y * temp->w + (int)x] = pixels[srcy * temp->w + srcx]; 
                
            //area mapping : use the first decimal of the double in order to
            // ponderate x,y ; x+1,y ; x,y+1 ; x+1,y+1.
            
            //currently debugging
 
            //fprintf(f,"[DEBUG] :\n NEWX = %i  | NEWY = %i  | OLDY = %f | OLDX = %f | Index = %i  \n", (int) srcx, (int) srcy, y, x, (int) (y*temp->w + x));
            
        }
    }

    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
}

SDL_Surface* RotateDetectedGrid(SDL_Surface* surface, Dot* dot1, Dot* dot2)
{
    if (dot1->x == dot2->x)
        errx(1,"RotateDetectedGrid: Length of the top line of the grid is 0.");

    if (dot1->x > dot2->x)
    {
        Dot* temp = dot1;
        dot1 = dot2;
        dot2 = temp;
    }

    double angle = atan(( dot2->y - dot1->y ) / ( dot2->x - dot1->x ));
    
    return rotateAny(surface,angle,""); 
}


