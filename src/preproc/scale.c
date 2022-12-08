#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "struct.h"

/**
 * recursive for Step2
**/
void Step2_rec(int* label, int h, int w, int i, int j, int l, int r)
{
    label[i * w + j] = l;
    if (r < 100000)
    {
        if (j + 1 < w - 1)
            if (label[i * w + j + 1] != 0 && label[i * w + j + 1] != l)
                Step2_rec(label, h, w, i, j + 1, l, r + 1);
        if (j - 1 > 1)
            if (label[i * w + j - 1] != 0 && label[i * w + j - 1] != l)
                Step2_rec(label, h, w, i, j - 1, l, r + 1);
        if (i + 1 < h - 1)
            if (label[(i + 1) * w + j] != 0 && label[(i + 1) * w + j] != l)
                Step2_rec(label, h, w, i + 1, j, l, r + 1);
        if (i - 1 > 1)
            if (label[(i - 1) * w + j] != 0 && label[(i - 1) * w + j] != l)
                Step2_rec(label, h, w, i - 1, j + 1, l, r + 1);
    }
}

/**
 * Description : labeling the grid (label (h * w)
**/
void Step2(int* label, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (label[i * w + j] != 0)
                Step2_rec(label, h, w, i, j, label[i * w + j], 0);
        }
    }
}

/** 
 * check if the cell must be labeled
 * return 
 *      lebel of the cell if the cell must be labeled
 *      -1 else
**/
int IsLabeled(int prev, int prev_top, Uint32 p, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(p, format, &r, &g, &b);
    if (r == (Uint8) 0)
        return 0;
    if (prev_top != 0)
        return prev_top;
    if (prev != 0)
        return prev;
    return -1;
}

/** 
 * Fill the array label_stats with the label array
 * label : array (h * w) with the differents label of the image
 * label_stats : array (nb Label)
 * Description : foreach label store the number of cells
**/
void fillStats(int* label, int* label_stats, int h, int w)
{
    for (int i = 1; i < h; i ++)
    {
        for (int j = 1; j < w; j++)
            if (label[i * w + j] != 0)
                label_stats[label[i * w + j]] += 1;
    }
}

/**
 * Fill everything else than the label in blue
 * label : grid (h * w) witch store the differents labels
 * l : label to save
**/
void add_color(Uint32* pixel, int* label, int l, int h, int w)
{
    int i = h - 1;
    int j = 0;
    while(i > 0 && label[i * w + j] != l)
    {
        while (j < w)
        {
            if (label[i * w + j] != l)
                pixel[i * w + j] = (Uint32) 255;
            else
                break;
            j += 1;
        }
        i -= 1;
        j = 0;
    }
    i = 0;
    j = w - 1;
    while(i < h && label[i * w + j] != l)
    {
        while (j > 0)
        {
            if (label[i * w + j] != l)
                pixel[i * w + j] = (Uint32) 255;
            else
                break;
            j -= 1;
        }
        i += 1;
        j = w - 1;
    }
}





/**
 * Descriptiom : find the coo of the label :
 * Find the right top angle
 * Find the right bottom angle
 * Find the left top angle
**/
void find_square(int* label, int h, int w, int l, Square* square)
{
    // Top Left
    Dot topLeft;
    int i = 0;
    int j = 0;
    while (i < h && j < w&& label[i * w + j] != l)
    {
        while (j < w && label[i * w + j] != l)
            j++;
        if (j <= w && label[i * w + j] != l)
        {
            i++;
            j = 0;
        }
    }
    topLeft.X = i;
    topLeft.Y = j;

    // Top Right
    Dot topRight;
    i = 0;
    j = w - 1;
    while (j > 0 && i < h && label[i * w + j] != l)
    {
        while (i < h - 1 && label[i * w + j] != l)
            i++;
        if (i < h && label[i * w + j] != l)
        {
            j--;
            i = 0;
        }
    }
    topRight.X =i;
    topRight.Y = j;

    // bottom Left
    Dot bottomLeft;
    i = h - 1;
    j = 0;
    while (j < w && i > 0 && label[i * w + j] != l)
    {
        while (i > 0 && label[i * w + j] != l)
            i--;
        if (i >= 0 && label[i * w + j] != l)
        {
            j++;
            i = h - 1;
        }
    }
    bottomLeft.X = i;
    bottomLeft.Y = j;

    // bottom Right
    Dot bottomRight;
    i = h - 1;
    j = w - 1;
    while (j > 0 && i > 0 && label[i * w + j] != l)
    {
        while (i > 0 && label[i * w + j] != l)
            i--;
        if (i >= 0 && label[i * w + j] != l)
        {
            j--;
            i = h - 1;
        }
    }
    bottomRight.X = i;
    bottomRight.Y = j;


    square->topLeft = topLeft;
    square->topRight = topRight;
    square->bottomLeft = bottomLeft;
    square->bottomRight = bottomRight;
}

void find_square2(int* label, int h, int w, int l, Square* square)
{
    // Top Left
    Dot topLeft;
    int i = 0;
    int j = 0;
    while (i < h && label[i * w + j] != l)
    {
        while (j < i && j < w && label[i * w + j] != l)
        {
            j++;
        }
        if (i < h && j < w && label[i * w + j] != l)
        {
            i++;
            j = 0;
        }
    }

    topLeft.X = i;
    topLeft.Y = j;

    // Top Right
    Dot topRight;
    i = 0;
    j = w - 1;
    while (i < h && label[i * w + j] != l)
    {
        while (j > i && j < w && label[i * w + j] != l)
        {
            j--;
        }
        if (i < h && j < w && label[i * w + j] != l)
        {
            i++;
            j = w - 1;
        }
    }

    topRight.X = i;
    topRight.Y = j;

    // Bottom Left
    Dot bottomLeft;
    i = h - 1;
    j = 0;

    while (j < w && label[i * w + j] != l)
    {
        while (i > j && i < h && label[i * w + j] != l)
        {
            i--;
        }
        if (i < h && j < w && label[i * w + j] != l)
        {
            j++;
            i = h - 1;
        }
    }

    bottomLeft.X = i;
    bottomLeft.Y = j;

    // Bottom Right
    Dot bottomRight;
    i = h - 1;
    j = w - 1;

    while (j > 0 && label[i * w + j] != l)
    {
        while (i > j && i > 0 && label[i * w + j] != l)
        {
            i--;
        }
        if (i >= 0 && j >= 0 && label[i * w + j] != l)
        {
            j--;
            i = h - 1;
        }
    }
    while (j + 5 < w && i < h && (label[i * w + j + 1] == l || label[i * w + j - w +
    5] == l))
    {
        if (label[i * w + j + 1] == l)
            j++;
        else if (label[i * w + j - w + 2] == l)
        {
            i--;
            j += 5;
        }
    }

    bottomRight.X = i;
    bottomRight.Y = j;

    square->topLeft = topLeft;
    square->topRight = topRight;
    square->bottomLeft = bottomLeft;
    square->bottomRight = bottomRight;
}

/**
 * Description : calculate the area of a square
**/
int Area(Square* square)
{
    double d1 = sqrt(pow(abs(square->topLeft.X - square->topRight.X), 2) +
                pow(abs(square->topLeft.Y - square->topRight.Y), 2));

    double d2 = sqrt(pow(abs(square->topLeft.X - square->bottomLeft.X), 2) +
                pow(abs(square->topLeft.Y - square->bottomLeft.Y), 2));

    return (int) (d1 * d2);
}

/**
 * find the index of the max of the array of label_stats and store it in *index
**/
void max(int* label_stats, int l, int* index)
{
    int max = label_stats[0];
    int max2 = label_stats[0];
    *index = 0;
    *(index + 1) = 0;
    for (int i = 0; i < l; i++)
    {
        if (label_stats[i] > max2)
        {
            if (label_stats[i] > max)
            {
                *(index + 1) = *index;
                max2 = max;
                max = label_stats[i];
                *index = i;
            }
            else
            {
                *(index + 1) = i;
                max2 = label_stats[i];
            }
        }
    }
}

/**
 * Find the grid of the imge
**/
void find_grid(SDL_Surface* surface, Square* s)
{
    Uint32* pixels = surface->pixels;
    int w = surface -> w;
    int h = surface -> h;// gets the length of pixels with the
                                      // width and length of the given surface.
    SDL_PixelFormat* format = surface->format;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int* label = calloc(w + 1, sizeof(int) * h + 1);
    int maxLabel = 1;

    // fill the label
    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int l = IsLabeled(label[i * w + j - 1], label[(i - 1) * w + j], 
                    pixels[i * w + j], format);
            if (l != -1)
                label[i * w + j] = l;
            else
                label[i * w + j] = maxLabel++;
        }
    }

    // fix the label fill
    Step2(label, h, w);

    // Stats about labels
    int* label_stats = calloc(maxLabel, sizeof(int));
    fillStats(label, label_stats, h, w);

    // find the 2 biggest label
    int big_label[] = {0, 0};
    max(label_stats, maxLabel, big_label);

    Square square1;
    Square square2;
    
    find_square(label, h, w, big_label[0], &square1);
    find_square(label, h, w, big_label[1], &square2);

    int a1 = Area(&square1);
    int a2 = Area(&square2);
    if (a1 > a2)
    {
    	find_square2(label, h, w, big_label[0], &square1);
        s->topLeft = square1.topLeft;
        s->topRight = square1.topRight;
        s->bottomLeft = square1.bottomLeft;
        s->bottomRight = square1.bottomRight;
        add_color(pixels, label, big_label[0], h, w);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 1] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 2] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 3] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 4] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 5] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 6] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 7] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 8] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 9] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 10] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 1] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 2] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 3] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 4] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 5] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 6] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 7] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 8] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 9] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y + 10] = SDL_MapRGB(format, 255, 0, 0);
    }
    else
    {
    	find_square2(label, h, w, big_label[1], &square2);
        s->topLeft = square2.topLeft;
        s->topRight = square2.topRight;
        s->bottomLeft = square2.bottomLeft;
        s->bottomRight = square2.bottomRight;
        add_color(pixels, label, big_label[1], h, w);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 1] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 2] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 3] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 4] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 5] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 6] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 7] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 8] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 9] = SDL_MapRGB(format, 255, 0, 0);
        pixels[s->bottomRight.X * w + s->bottomRight.Y - 10] = SDL_MapRGB(format, 255, 0, 0);
    }


    free(label_stats);
    free(label);
    SDL_UnlockSurface(surface);
}

void find_coin(SDL_Surface* surface, Square* s)
{
    Uint32* pixels = surface->pixels;
    int w = surface -> w;
    int h = surface -> h;// gets the length of pixels with the
                                      // width and length of the given surface.
    SDL_PixelFormat* format = surface->format;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int* label = calloc(w + 1, sizeof(int) * h + 1);
    int maxLabel = 1;

    // fill the label
    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            int l = IsLabeled(label[i * w + j - 1], label[(i - 1) * w + j], 
                    pixels[i * w + j], format);
            if (l != -1)
                label[i * w + j] = l;
            else
                label[i * w + j] = maxLabel++;
        }
    }

    // fix the label fill
    Step2(label, h, w);

    // Stats about labels
    int* label_stats = calloc(maxLabel, sizeof(int));
    fillStats(label, label_stats, h, w);

    // find the 2 biggest label
    int big_label[] = {0, 0};
    max(label_stats, maxLabel, big_label);

    Square square1;
    
    find_square(label, h, w, big_label[0], &square1);

    find_square2(label, h, w, big_label[0], &square1);
    s->topLeft = square1.topLeft;
    s->topRight = square1.topRight;
    s->bottomLeft = square1.bottomLeft;
    s->bottomRight = square1.bottomRight;
}

