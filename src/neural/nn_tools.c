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
    //return x > 0 ? x : 0;
}

/**
** Derived of sigmoid activation function
*/
double d_sigmoid(double x)
{
    return x * (1 - x);
    //return x < 0 ? 0 : 1;
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
** Generate random number between -1 and 1
*/
double get_rand_double()
{
    double range = 2;
    double div = RAND_MAX / range;
    return -1 + (rand() / div);
}

/**
** Shuffle array of size n
*/
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + (rand() * (n - i)) / RAND_MAX;
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
    char* path = malloc(sizeof(char) * 45);
    sprintf(path, "dataset/%i/%li.png", digit, n);

    // create SDL surface
    SDL_Surface* surface = IMG_Load(path);
    free(path);

    // number of bytes per pixel
    const Uint8 bpp = surface->format->BytesPerPixel;

    for (int i = 0; i < surface->w * surface->w; i++)
    {
        //if (!(i % 16))
        //    printf("\n");

        Uint8* pixel_p = (Uint8*) surface->pixels + i / surface->w
            * surface->pitch + i % surface->w * bpp;
        Uint32 pixel_data = *(Uint32*)pixel_p;

        SDL_Color color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

        // retrieve RGB value
        SDL_GetRGB(pixel_data, surface->format, &color.r, &color.g, &color.b);

        //bit_a[i] = (double) color.r;
        bit_a[i] = (double) (color.r / 255.0f);
        //printf("%i ", (int) bit_a[i]);
    }
    //printf("\n\n");
}


/**
** Determine most probable output
*/
char get_output(double* outputs)
{
    double max_v = 0;
    char res;

    for (char i = 0; i < 9; i++)
        if (*(outputs + i) > max_v)
        {
            max_v = *(outputs + i);
            res = i;
        }

    return res + 1;
}
