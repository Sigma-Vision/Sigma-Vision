#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../neutralize.h"
#include "../tools.h"
#include "../struct.h"
#include "../rotate.h"
#include "../scale.h"
#include "../struct.h"

int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: neutralize <image-file> <outfile>");

    // - Create a surface from the image.
    SDL_Surface* surface = load_image(argv[1]);

    // - Neutralizes the image and saves it.
    surface_to_grayscale(surface);

    //surface = GaussianBlur(surface,5);
    //rotate180(surface);

    OtsuBinarization(surface); 
    //surface = rotateAny(surface,45);

    surface = SobelTransform(surface);

    Square s;
    find_grid(surface, &s);
    
    surface = RotateDetectedGrid(surface,&(s.topLeft), &(s.topRight));
    
    SDL_Surface* newsurface = GridCropping(surface,&(s.topLeft),&(s.topRight));
    
    //need to free because not freed by GridCropping
    SDL_FreeSurface(surface);

    GridSplit(newsurface);
    //rotate180(surface);

    //surface = rotateAny(surface,-45,0);

    char* filename = argv[2];
    IMG_SaveJPG(newsurface,filename,100);
    // - Cleanup
    SDL_FreeSurface(newsurface);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
