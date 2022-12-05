#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "neutralize.h"
#include "transform.h"
#include "tools.h"
#include "struct.h"
#include "rotate.h"
#include "scale.h"
#include "transform.h"

int preproc(char* file, char* output)
{
    if (file == NULL)
        errx(EXIT_FAILURE, "Usage: neutralize <image-file> <outfile>");

    // - Create a surface from the image.
    SDL_Surface* surface = load_image(file);

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

    GridSplit(surface);

    IMG_SaveJPG(surface, output, 100);
    // - Cleanup
    SDL_FreeSurface(surface);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
