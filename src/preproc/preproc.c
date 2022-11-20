#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "neutralize.h"
#include "tools.h"
#include "struct.h"
#include "rotate.h"
#include "scale.h"

int preproc(int argc, char** argv)
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
  
    

    //rotate180(surface);

//  surface = rotateAny(surface,0,255,1);
    
    surface = RotateDetectedGrid(surface,&s); 

    SDL_Surface* to_free = surface;
    surface = GridCropping(to_free,&s);
    SDL_FreeSurface(to_free);

    char* filename = argv[2];
    IMG_SaveJPG(surface,filename,100);
    // - Cleanup
    SDL_FreeSurface(surface);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
