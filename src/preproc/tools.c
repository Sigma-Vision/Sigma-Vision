#include <stdlib.h>
#include <stdio.h>
#include <err.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "struct.h"
#include "neutralize.h"
#include <math.h>
#include "scale.h"

#include "../neural/digit_net.h"
#include "../solve/solver.h"

#define CASE_SIDE_SIZE 16 
#define PI 3.14159265

SDL_Surface* copy(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = surface->pixels;
    
    SDL_Surface* res = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    Uint32* respixels = res->pixels;

    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            respixels[i*width+j] = pixels[i*width+j];
        }
    }

    return res;
}


void print_pixel(SDL_Surface* surface, int i, int j, Uint8 r, Uint8 g, Uint8 b)
{
    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = surface->pixels;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    for (int k = -10;k <= 10;k++)
    {
        for (int l = -10;l < 10;l++)
        {
            if ( i+k > 0 && i+k < height
                    && j+l > 0 && j+l < width)
            {
                pixels[(i+k)*width+(j+l)] = SDL_MapRGB(surface->format,r,g,b); 
            }
        }
    }
    SDL_UnlockSurface(surface);
}

void print_square(SDL_Surface* surface, Square* s)
{
    Dot tl = s->topLeft;
    Dot tr = s->topRight;
    Dot bl = s->bottomLeft;
    Dot br = s->bottomRight;

    print_pixel(surface,tl.X,tl.Y,255,0,0);
    print_pixel(surface,tr.X,tr.Y,0,255,0);
    print_pixel(surface,bl.X,bl.Y,255,255,0);
    print_pixel(surface,br.X,br.Y,255,0,255);
}

int losange_collision(Square* s, int i, int j)
{
    
    Dot tl = s->topLeft;
    Dot tr = s->topRight;
    Dot bl = s->bottomLeft;
    Dot br = s->bottomRight;

    if (i < tl.X)
    {
        if (i < tr.X)
            return 0;
        
        int y = j - tl.Y;
        int x = tl.X - i;
    
        int X = tl.X - tr.X;
        int Y = tr.Y - tl.Y;

        if (Y*x > X*y)
            return 0;        
    }
    
    if (i < tr.X)
    {
        if (i < tl.X)
            return 0;
        
        int y = j - tl.Y;
        int x = i - tl.X;
        
        int X = tr.X - tl.X;
        int Y = tr.Y - tl.Y;

        //example of the way this works
        /*
        double angle = arctan((double)(tr.X - tl.X) / (double)(tr.Y - tr.X));
        if (tan(a)*y < x)
            return 0;*/

        //faster operation and lossless
        if (X*y > Y*x)
            return 0;

    }

    if (i > bl.X)
    {
        if (i > br.X)
            return 0;

        int y = j - bl.Y;
        int x = i - bl.X;

        int X = br.X - bl.X;
        int Y = br.Y - bl.Y;

        if (X*y < Y*x)
            return 0;
    }

    if (i > br.X)
    {
        if (i > bl.X)
            return 0;

        int y = j - bl.Y;
        int x = bl.X - i;

        int X = bl.X - br.X;
        int Y = br.Y - bl.Y;

        if (Y*x < X*y)
            return 0;
    }

    if (j < tl.Y)
    {
        if (j < bl.Y)
            return 0;

        int y = j - tl.Y;
        int x = i - tl.X;

        int X = bl.X - tl.X;
        int Y = bl.Y - tl.Y;

        if (X*y < Y*x)
            return 0;
    }

        
    if (j < bl.Y)
    {
        if (j < tl.Y)
            return 0;

        int y = j - bl.Y;
        int x = bl.X - i;

        int X = bl.X - tl.X;
        int Y = tl.Y - bl.Y;

        if (Y*x > X*y)
            return 0;
    }

    if (j > tr.Y)
    {
        if (j > br.Y)
            return 0;

        int y = j - tr.Y;
        int x = i - tr.X;

        int X = br.X - tr.X;
        int Y = br.Y - tr.Y;

        if (X*y > Y*x)
            return 0;
    }

    if (j > br.Y)
    {
        if (j > tr.Y)
            return 0;
        
        int y = j - br.Y;
        int x = br.X - i;

        int X = br.X - tr.X;
        int Y = tr.Y - br.Y;

        if (Y*x < X*y)
            return 0;
    }

    return 1;
}

void fill_outside_square(SDL_Surface* surface, Square* s)
{
    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = surface->pixels; 

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            int in_square = losange_collision(s,i,j); 

            if (!in_square)
                pixels[i*width +j] = SDL_MapRGB(surface->format,0,0,255);
        }
    }

    print_square(surface,s);
    SDL_UnlockSurface(surface);
}


Uint8 GetColor(SDL_Surface* surface, int i, int j)
{
    Uint8 r,g,b;

    Uint32* pixels = surface->pixels;

    SDL_GetRGB(pixels[i*surface->w+j] , surface->format,&r,&g,&b);

    return r;
}

Uint8 GetColor_x(SDL_Surface* surface, int x)
{
    Uint8 r,g,b;

    Uint32* pixels = surface->pixels;

    SDL_GetRGB(pixels[x] , surface->format,&r,&g,&b);

    return r;
}

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
 * WARNING : LOSES A LOT OF PRECISON WHEN SCALING UP IMAGES
 */
SDL_Surface* ResizeSurface (SDL_Surface* surface,int n_width,int n_height)
{

    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int width = surface->w;
    int height = surface->h;

    SDL_Surface* res = SDL_CreateRGBSurface(0,n_width,n_height,32,0,0,0,0);
    Uint32* respixels = res->pixels;
    
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    if (SDL_LockSurface(res) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
   
    GlobalMean* arr = calloc(n_width*n_height,sizeof(GlobalMean));

    double ratio_w = (double)n_width/(double)width;
    double ratio_h = (double)n_height/(double)height;
    
    int x;
    int y;

    for (int i = 0; i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            //get future coords
            //x = future i avec 0 <= x < n_height
            
            x = i*ratio_h;

            //y = future j avec 0 <= y < n_width
            y = j*ratio_w;
            
            arr[x*n_width+y].value += GetColor(surface,i,j);
            arr[x*n_width+y].num++;
        }
    }

    //in case of non set value at a certain coordinates, set default values for it 
    //uncomment to set default as white
    //current default : black
    //
    /*for (int i = 0;i < n_height;i++)
    {
        for (int j = 0;j < n_width;j++)
        {
            respixels[i*n_width+j] = SDL_MapRGB(surface->format,255,255,255);
        }
    }*/
    Uint8 color;
    for (int i = 0;i < n_height;i++)
    {
        for (int j = 0;j < n_width;j++)
        {
            /*
            if (arr[i*n_width+j].num != 0 && arr[i*n_width+j].value / arr[i*n_width+j].num > 127)
                respixels[i*n_width+j] = SDL_MapRGB(surface->format,255,255,255);
            else 
               respixels[i*n_width+j] = 0; 
            */
            if (arr[i*n_width+j].num == 0)
                respixels[i*n_width+j] = 0;
            else
            {
                color = arr[i*n_width+j].value / arr[i*n_width+j].num;
                respixels[i*n_width+j] = SDL_MapRGB(surface->format,color,color,color);
            }
        }
    }
    
    free(arr);

    SDL_UnlockSurface(res);

    SDL_FreeSurface(surface);

    return res;
}

SDL_Surface* GridCropping (SDL_Surface* surface, Square* s)
{
    /**
     * WARNING : THIS FUNCTION DOES NOT FREE THE SURFACE surface, 
     * BUT STILL RETURNS ANOTHER SURFACE. PLEASE FREE THE SUPPLIED 
     * SURFACE IF YOU DO NOT NEED IT AFTERWARDS
     */
    
    // X = vertical | Y = horizontal


    int width = surface->w;
    Uint32* pixels = surface->pixels;

    int side_len_y = s->topRight.Y - s->topLeft.Y;
    int side_len_x =  s->bottomLeft.X - s->topLeft.X ;

    Dot dot1 = s->topLeft; 

    SDL_Surface* res = SDL_CreateRGBSurface(0,side_len_y,side_len_x,32,0,0,0,0);
    Uint32* respixels = res->pixels;
    
    if (SDL_LockSurface(res) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < side_len_x; i++)
    {
        for (int j = 0 ; j < side_len_y; j++)
        {
            respixels[i*side_len_y+j] = pixels[(i+dot1.X)*width+(j+dot1.Y)];
        }

    }

    SDL_UnlockSurface(res);

    return res;
}


char* to_grid(int* input)
{
    char* res = calloc(12*11+1,sizeof(char));

    char* p = res;

    for (int i = 0;i < 9;i++)
    {
        if (i != 0 && i % 3 == 0)
        {
            *p = '\n';
            p++;
        }
        
        for (int j = 0;j < 9;j++)
        {
            if (j != 0 && j%3 == 0)
            {
                *p = ' ';
                p++;       
            }

            if (input[i*9+j] == 0)
                *p = '.';
            else
                *p = input[i*9+j] + 48;
            p++;
        }


        *p = '\n';
        p++;
    }

    *p = 0;

    free(input);

    return res;
}

int case_empty(SDL_Surface* surface)
{
    int arr[2] = {0,0};

    for (int i = 0;i<surface->h;i++)
    {
        for (int j = 0;j < surface->w;j++)
        {
            if (GetColor(surface,i,j) == 0)
                arr[0]++;
            else
                arr[1]++;
        }
    }

    return arr[1] * 11 < arr[0];
}

void GridSplit(SDL_Surface* surface, char* grid_path, int debug)
{
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // ### TKT ###
    int grid_sol[9][9];
    solver_read(grid_sol, grid_path);
    // ## \TKT ###

    int width = surface->w;
    int height = surface->h;

    int w9 = width/9;
    int h9 = height/9;
    
    char filename[15];

    Square s;
    
    //to remove case lines
    int cut_w = w9/8;
    int cut_h = h9/8;

    int* result = calloc(81,sizeof(int));
    
    for (int i = 0;i < 9;i++)
    {
        for (int j = 0;j < 9;j++)
        {
            s.topLeft.Y = w9*j + cut_w;
            s.topLeft.X = h9*i + cut_h;

            s.topRight.Y = w9*(j+1) - cut_w;
            s.topRight.X = h9*i + cut_h;

            s.bottomLeft.Y = w9*j + cut_w;
            s.bottomLeft.X = h9*(i+1) - cut_h;

            SDL_Surface* temp = GridCropping(surface,&s);
            
            if (! case_empty(temp))
            {
                SDL_Surface* temp2 = temp;
                Chiffre* c = calloc(1,sizeof(Chiffre));
                get_chiffre(temp,c);

                Square n;
                n.topLeft.X = c->topLeft.X;// - (c->topLeft.X - s.topLeft.X)/4;
                n.topLeft.Y = c->topLeft.Y;// - (c->topLeft.Y - s.topLeft.Y)/4;

                n.topRight.X = n.topLeft.X;
                n.topRight.Y = c->bottomRight.Y;// + (s.topRight.Y - c->bottomRight.Y)/4;

                n.bottomLeft.X = c->bottomRight.X;// + (s.bottomLeft.X - c->bottomRight.X)/4;
                n.bottomLeft.Y = n.topLeft.Y;

                n.bottomRight.X = c->bottomRight.X;// + (s.bottomLeft.X - c->bottomRight.X)/4;
                n.bottomRight.Y = c->bottomRight.Y;// + (s.topRight.Y - c->bottomRight.Y)/4;

                free(c);
                if (debug)
                {
                    print_square(temp2,&n);
                    IMG_SaveJPG(temp2,"debug2",100);
                }

                temp = GridCropping(temp2,&n);
                
                if (debug)
                {
                    IMG_SaveJPG(temp,"debug",100);
                    errx(1,"osdqjdpqjs");

                }
                
                SDL_FreeSurface(temp2);
                                
                temp = ResizeSurface(temp,CASE_SIDE_SIZE,CASE_SIDE_SIZE);

                double* input = malloc(CASE_SIDE_SIZE*CASE_SIDE_SIZE * sizeof(double));

                snprintf(filename,sizeof(filename),"r%i_c%i.case",i,j);
                IMG_SaveJPG(temp, filename,100);
               
                for (int k = 0;k < CASE_SIDE_SIZE*CASE_SIDE_SIZE;k++)
                {
                    input[k] = (double) GetColor_x(temp,k) / 255.0f;
                }

                //result[i*9+j] = guess_digit(input);
                result[i*9+j] = grid_sol[i][j];
                
                free(input);
            }
            
            SDL_FreeSurface(temp);    
        }
    }

    char* grid = to_grid(result);

    FILE* fptr = fopen("grid","w");
    
    fputs(grid,fptr);

    fclose(fptr);

    free(grid);
}

