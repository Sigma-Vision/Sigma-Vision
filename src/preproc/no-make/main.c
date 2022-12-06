#include "../preproc.h"
#include "../tools.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: neutralize <image-file> <outfile>");

    // - Create a surface from the image.
    SDL_Surface* surface = load_image(argv[1]);


    surface = preproc(surface);

    IMG_SaveJPG(surface, argv[2], 100);
    // - Cleanup
    SDL_FreeSurface(surface);
    SDL_Quit();
}
