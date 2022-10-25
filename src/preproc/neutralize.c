#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
    
    r = g = b = (r+g+b)/3; 
    
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
   
    int avg = 0.3*r + 0.59*g + 0.11*b;

    if (avg > 127)
        r = g = b = 255;
    else
        r = g = b = 0;

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
