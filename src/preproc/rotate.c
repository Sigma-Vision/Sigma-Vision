#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tools.h"
#include "struct.h"

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

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            npixels[(width-j-1)*height + i] = pixels[i*width+j]; 
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
    
    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
             npixels[j*height + (height - i - 1)] = pixels[i*width+j]; 
        }
    }

    SDL_FreeSurface(surface);

    SDL_UnlockSurface(temp);

    return temp;
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

SDL_Surface* rotateAny(SDL_Surface* surface,double angle,int color_fill,int rad)
{
    //width = x
    //height = y
    if (!rad)
        angle = angle*PI/180;
    // Angle from degrees to radian

    Uint32* pixels = surface -> pixels;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,surface->w,surface->h,32,0,0,0,0);
    double center_x = temp->w / 2;
    double center_y = temp->h / 2;
    
    Uint32* npixels = temp->pixels;

    if (SDL_LockSurface(temp) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32 color;

    switch (color_fill)
    {
        case 0:
            //blue
            color = SDL_MapRGB( temp->format, 0,0,255);
            break;
        case 1:
            //green
            color = SDL_MapRGB( temp->format, 0, 255, 0);
            break;
        case 2:
            //red
            color = SDL_MapRGB( temp->format, 255, 0, 0);
            break;
        case 3:
            //black
            color = SDL_MapRGB( temp->format, 0, 0, 0);
            break;
        case 4:
            //white
            color = SDL_MapRGB( temp->format, 255, 255, 255);
            break;
        default:
            // default case : color = blue
            color = SDL_MapRGB( temp->format, 0,0,255);
            break;
    }

    int srcx;
    int srcy;

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

SDL_Surface* RotateDetectedGrid(SDL_Surface* surface, Square* s)
{
    //X = VERTICAL
    //Y = HORIZONTAL

    Dot cdot1;
    cdot1.X = s->topLeft.X;
    cdot1.Y = s->topLeft.Y;

    Dot cdot2;
    cdot2.X = s->topRight.X;
    cdot2.Y = s->topRight.Y;

    double center_y = surface->w / 2;
    double center_x = surface->h / 2;

    if (s->topLeft.Y == s->topRight.Y)
        errx(1,"RotateDetectedGrid: Length of the top line of the grid is 0.");

    if (s->topLeft.Y > s->topRight.Y)
    {
        Dot temp = s->topLeft;
        cdot1.X = s->topRight.X;
        cdot1.Y = s->topRight.Y;
        cdot2.X = temp.X;
        cdot2.Y = temp.Y;
    }

    double angle = -atan((double)( cdot2.X - cdot1.X ) / (double)( cdot2.Y - cdot1.Y ));
    
    //we compute the future coordinates of the dots in order to not lose the 
    //square we found

    s->topLeft.Y = (cdot1.Y - center_y)*cos(angle) 
        - (cdot1.X - center_x) * sin(angle) + center_y;

    s->topLeft.X = (cdot1.Y - center_y) * sin(angle) 
        + (cdot1.X - center_x) * cos(angle) + center_x;
    
    s->topRight.Y = (cdot2.Y - center_y)*cos(angle) 
        - (cdot2.X - center_x) * sin(angle) + center_y;

    s->topRight.X = (cdot2.Y - center_y) * sin(angle) 
        + (cdot2.X - center_x) * cos(angle) + center_x;

    Dot cdot3 = s->bottomLeft; 
    Dot cdot4 = s->bottomRight;

    s->bottomLeft.Y = (cdot3.Y - center_y)*cos(angle) 
        - (cdot3.X - center_x) * sin(angle) + center_y;
    
    s->bottomLeft.X = (cdot3.Y - center_y) * sin(angle) 
        + (cdot3.X - center_x) * cos(angle) + center_x;
    
    s->bottomRight.Y = (cdot4.Y - center_y)*cos(angle) 
        - (cdot4.X - center_x) * sin(angle) + center_y;
    
    s->bottomRight.X = (cdot4.Y - center_y) * sin(angle) 
        + (cdot4.X - center_x) * cos(angle) + center_x;


    return rotateAny(surface,angle,0,1); 
}

/*int src_unlosange_left(Square* s,double angle,int i,int j, int width)
{
    Dot tl = s->topLeft;
    Dot bl = s->bottomLeft;
    Dot tr = s->topRight;

    int srcy = -(i-tl.X) * sin(angle) + (j-tl.Y) * cos(angle) + tl.Y;

    double coef = ((double)(j - tr.Y)/(double)(tl.Y - tr.Y)); 
    srcy = j + (int)((double)(srcy - j)*coef);
    
    if (srcy >= 0 && srcy < width)
        return srcy;
    else
        return j;
}
*/
