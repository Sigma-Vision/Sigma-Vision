#include <err.h>
#include <stdio.h>
#include <stdlib.h>
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

int get_X_center(int* label, int h, int w, int l)
{
    int pt1 = 0;
    int pt2 = h - 1;
    while (pt1 < h && label[pt1 * w + h / 2] != l)
        pt1++;
    while (pt2 > 0 && label[pt2 * w + h / 2] != l)
        pt2--;
    return (pt2 + pt1) / 2;
}

int get_Y_center(int* label, int h, int w, int l)
{
    int pt1 = 0;
    int pt2 = w - 1;
    while (pt1 < h && label[(w / 2) * w + pt1] != l)
        pt1++;
    while (pt2 > 0 && label[(w / 2) * w + pt2] != l)
        pt2--;
    return (pt2 + pt1) / 2;
}

int distance_X_Y(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
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
    while (i < h && j < w && label[i * w + j] != l)
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
    int X = get_X_center(label, h, w, l);
    int Y = get_Y_center(label, h, w, l);
    printf("center position : X = %i ; Y = %i\n", X, Y);

    int i = 0;
    int j = 0;
    int max = 0;
    int xmax = 0;
    int ymax = 0;
    while (i < X && j < Y)
    {
        if (label[i * w + j] == l && max < distance_X_Y(X, Y, i, j))
        {
            xmax = i;
            ymax = j;
            max = distance_X_Y(X, Y, i, j);
        }
        i ++;
        if (X == i)
        {
            i = 0;
            j++;
        }
    }
    Dot tl;
    tl.X = xmax;
    tl.Y = ymax;
    square->topLeft = tl;

    i = 0;
    j = w - 1;
    max = 0;
    while (i < X && j > Y)
    {
        if (label[i * w + j] == l)
        {
            if(max < distance_X_Y(X, Y, i, j))
            {
                xmax = i;
                ymax = j;
                max = distance_X_Y(X, Y, i, j);
            }
        }
        i ++;
        if (X == i)
        {
            i = 0;
            j--;
        }
    }
    Dot t2;
    t2.X = xmax;
    t2.Y = ymax;
    square->topRight = t2;

    i = h - 1;
    j = 0;
    max = 0;
    while (i > X && j < Y)
    {
        if (label[i * w + j] == l)
        {
            if(max < distance_X_Y(X, Y, i, j))
            {
                xmax = i;
                ymax = j;
                max = distance_X_Y(X, Y, i, j);
            }
        }
        j++;
        if (Y == j)
        {
            j = 0;
            i--;
        }
    }
    Dot t3;
    t3.X = xmax;
    t3.Y = ymax;
    square->bottomLeft = t3;

    i = h - 1;
    j = w - 1;
    max = 0;
    while (i > X && j > Y)
    {
        if (label[i * w + j] == l)
        {
            if(max < distance_X_Y(X, Y, i, j))
            {
                xmax = i;
                ymax = j;
                max = distance_X_Y(X, Y, i, j);
            }
        }
        i--;
        if (i == X)
        {
            i = h - 1;
            j--;
        }
    }
    Dot t4;
    t4.X = xmax;
    t4.Y = ymax;
    square->bottomRight = t4;
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
    }
    else
    {
        find_square2(label, h, w, big_label[1], &square2);
        s->topLeft = square2.topLeft;
        s->topRight = square2.topRight;
        s->bottomLeft = square2.bottomLeft;
        s->bottomRight = square2.bottomRight;
        add_color(pixels, label, big_label[1], h, w);
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

    free(label_stats);
    free(label);
    SDL_UnlockSurface(surface);
}

