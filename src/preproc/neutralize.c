#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "tools.h"

/**
 * value stay between 0 and 255
**/
int truncate(int value)
{
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

/**
 * Converts a colored pixel into grayscale.
 *
 * pixel_color: Color of the pixel to convert in the RGB format.
 * format: Format of the pixel used by the surface.
 */

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    r = g = b = (r+g+b) / 3;
    Uint32 color = SDL_MapRGB(format, r, g, b);
    
    return color;
}

/**
 * Converts a colored pixel to either a black or white pixel
 *
 * pixel_color: Color of the pixel to convert in the RGB format.
 * format: Format of the pixel used by the surface
 */

Uint32 binarize_pixel(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    //brightness
    int bright = 0;
    r = (Uint8) truncate((int) r - bright);
    g = (Uint8) truncate((int) g - bright);
    b = (Uint8) truncate((int) b - bright);

    //contrast
    int contrast = 0;
    float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
    r = (Uint8) truncate((int) (factor * (r - 128) + 128));
    g = (Uint8) truncate((int) (factor * (g - 128) + 128));
    b = (Uint8) truncate((int) (factor * (b - 128) + 128));

    int avg = 0.299 * r + 0.587 * g + 0.114 * b;
    //int avg = r+g+b/3;

/*    if (avg > 113)
        r = g = b = 0;
    else
        r = g = b = 255;
*/
    r = g = b = avg;
    Uint32 color = SDL_MapRGB(format, r, g, b);
    
    return color;
}

/**
 * Applies pixel_to_grayscale() to every pixel in the given surface.
 *
 * surface: Surface to be neutralized/grayscaled.
 */

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;// gets the length of pixels with the
                                      // width and length of the given surface.
    SDL_PixelFormat* format = surface->format;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0;i<len;i++)
    {
        pixels[i] = binarize_pixel(pixels[i],format);
    }

    SDL_UnlockSurface(surface);
}

int* OtsuBuildHistogram(SDL_Surface* surface)
{
    int* histogram = calloc(256,sizeof(int));

    int width = surface->w;
    int height = surface->h;

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width; j++)
        {
            //printf("working for : I = %i and J = %i\n",i,j);
            int value = GetColor(surface,i,j);
           
            if (value >= 256)
                errx(1,"Image has color value above 256");

            histogram[value] += 1;
            //printf("%i\n",histogram[value]); 
            //printf("working for : I = %i and J = %i\n",i,j);
        }
        //printf("out");
    }
    return histogram;
}

int OtsuGetMaxVariance(SDL_Surface* surface)
{
    int full_w = 0;

    int* histogram = OtsuBuildHistogram(surface);
    
    for (int i = 0;i < 256;i++)
    {
        full_w += histogram[i];
    }

    int Wb;
    int Wf;

    int mub;
    int muf;
    int mu;

    int variance;

    int max_variance = 0;
    int imax_variance = 0;

    //if i = 0 or i = 255, we have either Wb or Wf = 0 so variance = 0
    for (int i = 1;i < 255; i++)
    {
        Wb = 0;
        mub = 0;

        for (int k = 0;k < i;k++)
        {
           Wb += histogram[k];
           mub += histogram[k]*k;
        }
        
        mub /= Wb;
        Wb /= full_w; 

        Wf = 0;
        muf = 0;
        
        for (int k = i;k < 256;k++)
        {
            Wf += histogram[k];
            muf += histogram[k]*k;
        }
        
        muf /= Wf;
        Wf /= full_w;
    
        mu = mub - muf;

        variance = Wb*Wf*mu*mu;

        if (variance > max_variance)
        {
            imax_variance = i; 
            max_variance = variance;
        }
    }
    
    free(histogram);

    return imax_variance;
} 

void OtsuBinarization(SDL_Surface* surface)
{

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int color_max_variance = OtsuGetMaxVariance(surface);
     
    Uint32* pixels = surface-> pixels;
    SDL_PixelFormat* format = surface->format;

    for (int i = 0; i < surface -> h; i++)
    {
        for (int j = 0; j < surface -> w; j++)
        {
            if (GetColor(surface,i,j) > color_max_variance)
                pixels[i*surface->w+j] = SDL_MapRGB(format, 255, 255, 255);
            else
                pixels[i*surface->w+j] = SDL_MapRGB(format, 0, 0, 0);
        }
    } 

    SDL_UnlockSurface(surface);
}
