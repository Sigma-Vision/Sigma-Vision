#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/** 
 * Loads an image in a surface.
 * The format of the surface is SDL_PIXELFORMAT_RGB888.
 *
 * path: Path of the image.
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
    
    r = g = b = 0.3*r + 0.59*g + 0.11*b;
    
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
        pixels[i] = pixel_to_grayscale(pixels[i],format);
    }

    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: neutralize <image-file> <outfile>");

    // - Create a surface from the image.
    SDL_Surface* surface = load_image(argv[1]);

    // - Neutralizes the image and saves it.
    surface_to_grayscale(surface);
    char* filename = argv[2];
    IMG_SaveJPG(surface,filename,100);
    
    // - Cleanup
    SDL_FreeSurface(surface);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
