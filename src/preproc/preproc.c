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

SDL_Surface* preproc(SDL_Surface* surface, char* grid_path)
{
    // - Neutralizes the image and saves it.
    surface_to_grayscale(surface);

    OtsuBinarization(surface,0); 

    IMG_SaveJPG(surface,"binarized",100);

    //surface = Erosion(surface,1); 
    //surface = Dilation(surface,2);
    
    //surface = rotate90cw(surface);

    SDL_Surface* sobeled = SobelTransform(surface,5);
    //sobeled = Erosion(sobeled,3);
    //IMG_SaveJPG(sobeled, "sobel1.jpg", 100);
    
    Square s;
    find_grid(sobeled, &s);
    
    print_square(sobeled,&s); 
    IMG_SaveJPG(sobeled, "sobeled.jpg", 100);

    SDL_FreeSurface(sobeled); 


    surface = RotateDetectedGrid(surface,&s); 
    surface = unlosange(surface,&s);

    //print_square(surface,&s);
    //IMG_SaveJPG(surface,"preproc.jpg",100);

    SDL_Surface* to_free = surface;
    surface = GridCropping(to_free,&s);
    SDL_FreeSurface(to_free);

    GridSplit(surface,grid_path,0);
    return surface;
}
