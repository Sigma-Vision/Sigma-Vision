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

SDL_Surface* preproc(SDL_Surface* surface)
{
    // - Neutralizes the image and saves it.
    surface_to_grayscale(surface);

    OtsuBinarization(surface); 
 
    surface = Erosion(surface,1); 
    surface = Dilation(surface,2);

    SDL_Surface* sobeled = SobelTransform(surface);
    
    Square s;
    find_grid(sobeled, &s);

    print_square(sobeled,&s); 
    IMG_SaveJPG(sobeled, "sobeled.jpg", 100);

    SDL_FreeSurface(sobeled); 

    surface = RotateDetectedGrid(surface,&s); 
    
    SDL_Surface* to_free = surface;
    surface = GridCropping(to_free,&s);
    SDL_FreeSurface(to_free);

    GridSplit(surface);
    
    return surface;
}
