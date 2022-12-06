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
/*
    SDL_Surface* to_free = surface;
    surface = GridCropping(to_free,&s);
    SDL_FreeSurface(to_free);

    GridSplit(surface);
*/

    
    return surface;
}
