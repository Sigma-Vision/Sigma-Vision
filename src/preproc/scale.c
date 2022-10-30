#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "struct.h"

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

int isSquare(int* label, int h, int w, int i, int j, int l)
{
    int i1 = i;
    while (i1 < h && label[i1 * w + j] != l)
        i1 += 1;
    if (i1 >= h || label[i1 * w + j] != l)
        return 0;

    int j1 = j;
    while (j1 < w && label[i * w + j1] != l)
        j1 += 1;
    if (j1 >= w || label[i * w + j1] != l)
        return 0;

    i1 = i - 1;
    while (i1 > 0 && label[i1 * w + j] != l)
        i1 -= 1;
    if (i1 < 0 || label[i1 * w + j] != l)
        return 0;

    j1 = j;
    while (j1 > 0 && label[i * w + j1] != l)
        j1 -= 1;
    if (j1 < 0 || label[i * w + j1] != l)
        return 0;
    return 1;
}

void fillSquare(int* label, int i, int j, int h, int w, int l)
{
    int i1 = i;
    while (i1 < h && label[i1 * w + j] != l)
    {
        i1 += 1;
        label[i1 * w + j] = l;
    }

    int j1 = j;
    while (j1 < h && label[i * w + j1] != l)
    {
        j1 += 1;
        label[i * w + j1] = l;
    }
}

int fillLabel(int* label, int* bigLabel, int h, int w)
{
    int l1 = bigLabel[0];
    int l2 = bigLabel[1];
    printf("fill label : %i, %i", l1, l2);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w - 1; j++)
        {
            if (label[i * w + j] == l1 && label[i * w + j + 1] != l1)
                if (isSquare(label, h, w, i, j + 1, l1))
                    fillSquare(label, i, j + 1, h, w, l1);
            if (label[i * w + j] == l2 && label[i * w + j + 1] != l2)
                if (isSquare(label, h, w, i, j + 1, l2))
                    fillSquare(label, i, j + 1, h, w, l2);
        }
    }
}

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

int fillStats(int* label, int* label_stats, int h, int w)
{
    for (int i = 1; i < h; i ++)
    {
        for (int j = 1; j < w; j++)
            if (label[i * w + j] != 0)
                label_stats[label[i * w + j]] += 1;
    }
}

void find_coo(Uint32* pixel, int* label, int l, int h, int w)
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

    // Top Left
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


    square->topLeft = topLeft;
    square->topRight = topRight;
    square->bottomLeft = bottomLeft;
}

int Area(Square* square)
{
    double d1 = sqrt(pow(abs(square->topLeft.X - square->topRight.X), 2) +
                pow(abs(square->topLeft.Y - square->topRight.Y), 2));

    double d2 = sqrt(pow(abs(square->topLeft.X - square->bottomLeft.X), 2) +
                pow(abs(square->topLeft.Y - square->bottomLeft.Y), 2));

    printf("Pt 1 : %i, %i ; Pt 2 : %i, %i ; Pt 3 : %i, %i\n", square->topLeft.X,
        square->topLeft.Y, square->topRight.X, square->topRight.Y, 
         square->bottomLeft.X, square->bottomLeft.Y);

    printf("Distance : %i ; %i \n", (int) d1, (int) d2);
    printf("Area : %i\n\n", (int) (d1 * d2));
    return (int) (d1 * d2);
}

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

Square* find_grid(SDL_Surface* surface)
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
    printf("Label 1 : %i ; Label 2 : %i\n\n", big_label[0], big_label[1]);

    Square square1;
    Square square2;
    
    find_square(label, h, w, big_label[0], &square1);
    find_square(label, h, w, big_label[1], &square2);

    int a1 = Area(&square1);
    int a2 = Area(&square2);
    printf("Area 1 : %i, Area 2 : %i\n", a1, a2);
    if (a1 > a2)
    {
        find_coo(pixels, label, big_label[0], h, w);
        printf("Label : %i\n", big_label[0]);
    }
    else
    {
        find_coo(pixels, label, big_label[1], h, w);
        printf("Label : %i\n", big_label[1]);
    }

    // fill inside the label
    //fillLabel(label, big_label, h, w);

    // stats to 0
    //for (int i = 0; i < maxLabel; i ++)
      //  label_stats[i] = 0;


    //fillStats(label, label_stats, h, w);
   // big_label[0] = 0;
    //max(label_stats, maxLabel, big_label);


    free(label_stats);
    free(label);
    SDL_UnlockSurface(surface);
    
    return a1 >= a2 ? &square1 : &square2;
}

