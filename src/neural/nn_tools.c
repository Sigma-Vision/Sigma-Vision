#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
** Basic sigmoid activation function
*/
double sigmoid(double x)
{   
    return 1 / (1 + exp(-x));
}

/**
** Derived of sigmoid activation function
*/
double d_sigmoid(double x)
{   
    return x * (1 - x);
}

/**
** Init random number generation
*/
void init_rand()
{
    srand(time(NULL));
}

/**
** Generate random number long between 0 and n-1
*/
long get_rand_long(long n)
{
    return (long) rand() % n;
}

/**
** Generate random number between 0 and 1
*/
double get_rand_double()
{
    return ((double)rand()) / ((double)RAND_MAX);
}

/**
** Shuffle array of size n
*/
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;

        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + (rand() * (n - i + 1)) / RAND_MAX;
            int tmp = array[j];

            array[j] = array[i];
            array[i] = tmp;
        }
    }
}


/**
** Allocate memory and init values
*/
void init_params(const int nb_inputs, const int nb_layers, const int nb_nodes[],
                 double** nodes_pp, double** deltas_pp, double** biases_pp,
                 double*** weights_ppp, int learn)
{
    int prev_nb_nodes;
    int curr_nb_nodes = nb_inputs;

    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        prev_nb_nodes = curr_nb_nodes;
        curr_nb_nodes = nb_nodes[layer_i];

        int layer_mem_size = curr_nb_nodes * sizeof(double);

        *(nodes_pp + layer_i) = calloc(curr_nb_nodes, sizeof(double));
        if (deltas_pp)
            *(deltas_pp + layer_i) = calloc(curr_nb_nodes, sizeof(double));
        double* biases_p = malloc(layer_mem_size);
        double** weights_pp = malloc(layer_mem_size);
        *(biases_pp + layer_i) = biases_p;
        *(weights_ppp + layer_i) = weights_pp;

        int node_mem_size = prev_nb_nodes * sizeof(double);
        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
        {
            double* weights_p = malloc(node_mem_size);
            *(weights_pp + node_i) = weights_p;

            if (learn)
            {
                *(biases_p + node_i) = get_rand_double();

                for (int prev_node_i = 0; prev_node_i < prev_nb_nodes; prev_node_i++)
                    *(weights_p + prev_node_i) = get_rand_double();
            }
            else
            {
                *(biases_p + node_i) = 0;

                for (int prev_node_i = 0; prev_node_i < prev_nb_nodes; prev_node_i++)
                    *(weights_p + prev_node_i) = 0;
            }
        }

    }
}

void free_memory(const int nb_layers, const int nb_nodes[], double** nodes_pp,
                 double** deltas_pp, double** biases_pp, double*** weights_ppp)
{
    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        int curr_nb_nodes = nb_nodes[layer_i];

        free(*(nodes_pp + layer_i));

        if (deltas_pp)
            free(*(deltas_pp + layer_i));

        free(*(biases_pp + layer_i));

        double** weights_pp = *(weights_ppp + layer_i);
        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            free(*(weights_pp + node_i));
        free(weights_pp);
    }


    free(nodes_pp);
    if (deltas_pp)
        free(deltas_pp);
    free(biases_pp);
    free(weights_ppp);
}

/**
** Extract bit array from picture updating double* inputs values
** digit: value represented in picture (1-9)
** n: file identifier number
*/
void get_bit_array(int digit, long n, double bit_a[])
{
    // retrieve path
    // TODO modif path to architecture
    char* path = malloc(sizeof(char) * 10);
    sprintf(path, "%i%li.png", digit, n);

    // create SDL surface
    SDL_Surface* surface = IMG_Load(path);
    free(path);

    // number of bytes per pixel
    const Uint8 bpp = surface->format->BytesPerPixel;

    for (int i = 0; i < surface->w * surface->w; i++)
    {
        Uint8* pixel_p = (Uint8*) surface->pixels + i / surface->w
            * surface->pitch + i % surface->w * bpp;
        Uint32 pixel_data = *(Uint32*)pixel_p;

        SDL_Color color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

        // retrieve RGB value
        SDL_GetRGB(pixel_data, surface->format, &color.r, &color.g, &color.b);

        if ((color.r > 200) || (color.g > 200) || (color.b > 200))
            bit_a[i] = 1.0f;
    }
}


/**
** Extract bit array from picture updating double* inputs values
** digit: value represented in picture (1-9)
** n: file identifier number
*/
void get_inputs(int digit, long n, double inputs[])
{
    char* path = malloc(sizeof(char) * 10);
    sprintf(path,·
}
