#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

SDL_Surface* rebuild (char* not_solved, char* solved)
{
    SDL_Surface* number[] = 
    {IMG_Load("resources/1.png"),
        IMG_Load("resources/2.png"),
        IMG_Load("resources/3.png"),
        IMG_Load("resources/4.png"),
        IMG_Load("resources/5.png"),
        IMG_Load("resources/6.png"),
        IMG_Load("resources/7.png"),
        IMG_Load("resources/8.png"),
        IMG_Load("resources/9.png")};

    SDL_Surface* number_added[] =
    {IMG_Load("resources/1_a.png"),
        IMG_Load("resources/2_a.png"),
        IMG_Load("resources/3_a.png"),
        IMG_Load("resources/4_a.png"),
        IMG_Load("resources/5_a.png"),
        IMG_Load("resources/6_a.png"),
        IMG_Load("resources/7_a.png"),
        IMG_Load("resources/8_a.png"),
        IMG_Load("resources/9_a.png")};

    SDL_Rect dest;
    dest.x = 5;
    dest.y = 5;
    dest.w = 45;
    dest.h = 45;

    SDL_Surface* grid = IMG_Load("resources/blank-sudoku-grid.png");

    char *grid_ns = malloc(sizeof(char)*81);
    char *grid_s = malloc(sizeof(char)*81);

    FILE* ptr1;
    FILE* ptr2;

    ptr1 = fopen(not_solved, "r");
    if (ptr1 == NULL)
        errx(EXIT_FAILURE, "File GNS can't be opened \n");

    int n = 0;
    while (!feof(ptr1))
    {
        char ch = fgetc(ptr1);
        if (ch != '\n' && ch != ' ')
        {
            grid_ns[n] = ch;
            n++;
        }
    }
    fclose(ptr1);

    ptr2 = fopen(solved, "r");

    if (ptr2 == NULL)
        errx(EXIT_FAILURE, "File GS can't be opened \n");

    n = 0;
    while (!feof(ptr2))
    {
        char ch = fgetc(ptr2);
        if (ch != '\n' && ch != ' ')
        {
            grid_s[n] = ch;
            n++;
        }
    }
    fclose(ptr2);

    for (int i = 0; i < 81; i++)
    {
        int j = grid_s[i] - 49;
        if (grid_ns[i] != 46)
        {
            SDL_BlitSurface(number[j], NULL, grid, &dest);
        }
        else
        {
            SDL_BlitSurface(number_added[j], NULL, grid, &dest);
        }
        dest.x += 55;
        if (dest.x > 495)
        {
            dest.x = 5;
            dest.y += 55;
        }
    }

    for (size_t i = 0; i < 9 ; i++)
        SDL_FreeSurface(number[i]);
    for (size_t i = 0; i < 9 ; i++)
        SDL_FreeSurface(number_added[i]);
    return grid;
}
