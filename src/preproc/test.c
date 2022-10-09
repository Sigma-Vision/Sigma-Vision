#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "neutralize.h"
#include "tools.h"

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
