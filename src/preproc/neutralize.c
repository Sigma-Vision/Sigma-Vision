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

    //r = g = b = (r+g+b) / 3;
    r = g = b = 0.3*r + 0.59*g + 0.11*b;
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
        //pixels[i] = binarize_pixel(pixels[i],format);
        pixels[i] = pixel_to_grayscale(pixels[i],format);
    }

    SDL_UnlockSurface(surface);
}

void OtsuNormalizeHistogram(int* histogram)
{
    int min_intensity = 0;
    int max_intensity = 255;

    for (; min_intensity< 256 && histogram[min_intensity] == 0; min_intensity++);
    for (; max_intensity >= 0 && histogram[max_intensity] == 0; max_intensity--);

    if (max_intensity == 0)
        errx(4,"Histogram empty in OtsuNormalizeHistogram");

    for (int i = 0; i < 256;i++)
    {
        histogram[i] = ((histogram[i]-min_intensity)/(max_intensity - min_intensity)) * 255; 
    }
}

void OtsuEqualizeImage (SDL_Surface* surface, int* histogram,double nb_pix)
{
    if (nb_pix == 0)
        errx(4,"Histogram not build in OtsuEqualizeHistogram");
    
    int* color_map = malloc(sizeof(int)*256);
    
    double cumulated = 0;
    double val;
    for (int i = 0;i < 256;i++)
    {
        cumulated += histogram[i];
        val = cumulated * 255 / nb_pix;
        if (val - (double)(int)val >= 0.5)
           color_map[i] = (int)val+1;
        else
           color_map[i] = (int)val; 
    }

    Uint32* pixels = surface->pixels;
    int width = surface->w;

    for (int i = 0; i < surface->h;i++)
    {
        for (int j = 0; j < surface->w;j++)
        {
            int color = GetColor(surface,i,j);
            pixels[i*width+j] = SDL_MapRGB(surface->format,
                    color_map[color],color_map[color],color_map[color]);
        }
    }

    free(color_map);
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
            int value = GetColor(surface,i,j);
           
            if (value >= 256)
                errx(1,"Image has color value above 256");

            histogram[value] += 1;
        }
    }

    return histogram;
}


int OtsuGetMaxVariance(SDL_Surface* surface,int equalize)
{
    int full_w = surface->w * surface->h;

    int* histogram = OtsuBuildHistogram(surface);

    if (equalize)
    {
        SDL_Surface* egalized = copy(surface);

        OtsuEqualizeImage(egalized,histogram,full_w);
        
        free(histogram);

        histogram = OtsuBuildHistogram(egalized); 
        SDL_FreeSurface(egalized);

    }
    else
        OtsuNormalizeHistogram(histogram);

    double Wb;
    double Wf;

    double mub;
    double muf;
    double mu;

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

        variance = (int)(Wb*Wf*mu*mu);

        if (variance > max_variance)
        {
            imax_variance = i; 
            max_variance = variance;
        }
    }
    
    free(histogram);

    return imax_variance;
} 

void OtsuBinarization(SDL_Surface* surface,int equalize)
{

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int color_max_variance = OtsuGetMaxVariance(surface,equalize);
    
    Uint32* pixels = surface-> pixels;
    SDL_PixelFormat* format = surface->format;

    for (int i = 0; i < surface -> h; i++)
    {
        for (int j = 0; j < surface -> w; j++)
        {
            if (GetColor(surface,i,j) < color_max_variance)
                pixels[i*surface->w+j] = SDL_MapRGB(format, 255, 255, 255);
            else
                pixels[i*surface->w+j] = SDL_MapRGB(format, 0, 0, 0);
        }
    } 

    SDL_UnlockSurface(surface);
}

/**
 * Dilates an already binarized surface.
 */

SDL_Surface* Dilation(SDL_Surface* surface, int iterations)
{
    int width = surface->w;
    int height = surface->h;
    for (int iter = 0;iter < iterations;iter++)
    {

        SDL_PixelFormat* format = surface->format;

        SDL_Surface* res = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);

        if (SDL_LockSurface(res) < 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());
    
        Uint32* respixels = res-> pixels;

        for (int i = 0;i < height;i++)
        {
            for (int j = 0;j < width;j++)
            {
                if (GetColor(surface,i,j) == 255)
                {
                        if (i > 0)
                            respixels[(i-1)*width+j] = SDL_MapRGB(format,255,255,255);
                        if (i+1 < height)
                            respixels[(i+1)*width+j] = SDL_MapRGB(format,255,255,255);
                        if (j > 0)
                            respixels[i*width+(j-1)] = SDL_MapRGB(format,255,255,255);
                        if (j+1 >= width)
                            respixels[i*width+(j+1)] = SDL_MapRGB(format,255,255,255);
                }
                else
                {
                    respixels[i*width+j] = 0;
                } 
            }
        } 

        SDL_UnlockSurface(res);

        SDL_FreeSurface(surface);

        surface = res;
 
    }
    
    return surface;
}


SDL_Surface* Erosion(SDL_Surface* surface, int iterations)
{
    int width = surface->w;
    int height = surface->h;
    SDL_PixelFormat* format = surface->format;

    for (int iter = 0;iter < iterations;iter++)
    {
        SDL_Surface* res = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);

        if (SDL_LockSurface(res) < 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());
    
        Uint32* respixels = res-> pixels;

        for (int i = 0;i < height;i++)
        {
            for (int j = 0;j < width;j++)
            {
                if (GetColor(surface,i,j) == 255)
                {
                    //on disc == is not on border
                    int is_border = ! ((i<=0 || GetColor(surface,i-1,j) == 255) 
                            && (i+1 >= height || GetColor(surface,i+1,j) == 255) 
                            && (j <= 0 || GetColor(surface,i,j-1) == 255)
                            && (j+1 >= width || GetColor(surface,i,j+1) == 255));
                    if (is_border)
                    {
                        char found = 0; 
                    
                        if (i > 0 && GetColor(surface,i-1,j) == 255 )
                        {
                            found  = 1;
                            respixels[(i-1)*width+j] = 0;
                        }

                        if (i+1 < height && GetColor(surface,i+1,j) == 255)
                        {
                            found = 1;
                            respixels[(i+1)*width+j] = 0;
                        }

                        if (j > 0 && GetColor(surface,i,j-1) == 255)
                        {
                            found = 1;
                            respixels[i*width+(j-1)] = 0;
                        }

                        if (j+1 >= width && GetColor(surface,i,j+1) == 255)
                        {
                            found = 1;
                            respixels[i*width+(j+1)] = 0;
                        }

                        if (!found)
                            respixels[i*width+j] = 0;
                    }
                    else
                        respixels[i*width+j] = SDL_MapRGB(format,255,255,255);
                }
                else
                   respixels[i*width+j] = 0; 
            }
        } 

        SDL_UnlockSurface(res);

        SDL_FreeSurface(surface);
        
        surface = res;
    }
    
    return surface;
}

/*SDL_Surface* unlosange(SDL_Surface* surface,Square* s)
{
    int width = surface->w;
    int height = surface->h;

    Dot tl = s->topLeft;
    Dot tr = s->topRight;
    Dot bl = s->bottomLeft;
    Dot br = s->bottomRight;

    //do rotation as described in the picture
    //include ponderation with distance to the angle / distance to the dot that is off / further outside    
}*/
