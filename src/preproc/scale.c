#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void Step2_rec(int* label, int h, int w, int r, int c, int l)
{
    label[r * w + c] = l;
    if (c + 1 < w)
        if (label[r * w + c + 1] != 0 && label[r * w + c + 1] != l)
            Step2_rec(label, h, w, r, c + 1, l);
    if (c - 1 > 0)
        if (label[r * w + c - 1] != 0 && label[r * w + c - 1] != l)
            Step2_rec(label, h, w, r, c - 1, l);
    if (r + 1 < h)
        if (label[(r + 1) * w + c] != 0 && label[(r + 1) * w + c] != l)
            Step2_rec(label, h, w, r + 1, c, l);
    if (r - 1 > 0)
        if (label[(r - 1) * w + c] != 0 && label[(r - 1) * w + c] != l)
            Step2_rec(label, h, w, r - 1, c + 1, l);
}

void Step2(int* label, int h, int w)
{
    for (int i = 1; i < h - 1; i++)
    {
        for (int j = 1; j < w - 1; j++)
        {
            if (label[i * w + j] != 0)
                Step2_rec(label, h, w, i, j, label[i * w + j]);
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
    int i = 0;
    int j = 0;
    while(i < h && label[i * w + j] != l)
    {
        while (j < w)
        {
            if (label[i * w + j] != l)
                pixel[i * w + j] = (Uint32) 255;
            else
                break;
            j += 1;
        }
        i += 1;
        j = 0;
    }
    i = h - 1;
    j = w - 1;
    while(i > 0 && label[i * w + j] != l)
    {
        while (j > 0)
        {
            if (label[i * w + j] != l)
                pixel[i * w + j] = (Uint32) 255;
            else
                break;
            j -= 1;
        }
        i -= 1;
        j = w;
    }
}

int max(int* label_stats, int l)
{
    int max = label_stats[0];
    int index = 0;
    for (int i = 0; i < l; i++)
    {
        if (label_stats[i] > max)
        {
            max = label_stats[i];
            index = i;
        }
    }
    return index;
}

void find_grid(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int w = surface -> w;
    int h = surface -> h;// gets the length of pixels with the
                                      // width and length of the given surface.
    SDL_PixelFormat* format = surface->format;

    if (SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int* label = calloc(w, sizeof(int) * h);
    int maxLabel = 1;
    for (int i = 1; i < h; i++)
    {
        for (int j = 1; j < w; j++)
        {
            int l = IsLabeled(label[i * w + j - 1], label[(i - 1) * w + j], 
                    pixels[i * w + j], format);
            if (l != -1)
                label[i * w + j] = l;
            else
                label[i * w + j] = maxLabel++;
        }
    }

    Step2(label, h, w);
    /*for (int i = h - 1; i > 2; i--)
    {
        for (int j = w - 1; j > 2; j--)
        {
            if (label[i * w + j] != 0)
            {
                if (label[(i - 1) * w + j] != 0)
                    label[(i - 1) * w + j] = label[i * w + j];
                if (label[(i + 1) * w + j] != 0)
                    label[(i + 1) * w + j] = label[i * w + j];
                if (label[i * w + j - 1] != 0)
                    label[i * w + j - 1] = label[i * w + j];
                if (label[i * w + j + 1] != 0)
                    label[i * w + j + 1] = label[i * w + j];
                if (label[(i - 1) * w + j - 1] != 0)
                    label[(i - 1) * w + j - 1] = label[i * w + j];
                if (label[(i + 1) * w + j + 1] != 0)
                    label[(i + 1) * w + j + 1] = label[i * w + j];
            }
        }
    }*/

    int* label_stats = calloc(maxLabel, sizeof(int));

    fillStats(label, label_stats, h, w);
    int big_label = max(label_stats, maxLabel);
    printf("%i\n", big_label);
    find_coo(pixels, label, big_label, h, w);
    for (int i = 0; i < maxLabel; i ++)
        if (label_stats[i] > 1000)
            printf("%i ", label_stats[i]);

    free(label_stats);
    free(label);
    SDL_UnlockSurface(surface);
}

