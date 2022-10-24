#include <stdio.h>
#include <stdlib.h>
#include "config_manager.h"

void write_config(int nb_inputs, int nb_layers, int* nb_nodes_p, int** biases_pp,
                 int*** weights_ppp)
{
    FILE* file_p = fopen(CONFIG_FILE, 'w');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    // SUMMARY block
    fprintf(file_p, "# SUMMARY\n");
    // number of layers (hidden + 1 output)
    fprintf(file_p, "l %i\n", nb_layers);
    // number of inputs and nodes per layer
    fprintf(file_p, "s %i", nb_inputs);
    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
        fprintf(file_p, " %i", *(nb_nodes_p + layer_i));

    // DATA block
    fprintf(file_p, "\n# DATA");
    for (int layer_i = 1; layer_i < nb_layers; i++)
    {
        int nb_nodes = *(nb_nodes_p + layer_i);
        int nb_nodes_prev = *(nb_nodes_p + layer_i - 1);

        // biases
        fprintf(file_p, "\nb");
        for (int node_i = 0; node_i < nb_nodes; node_i++)
            fprintf(file_p, " %f", *(*(biases_pp + layer_i) + node_i));

        // weights
        fprintf(file_p, "\nw");
        for (int node_i = 0; node_i < nb_nodes; node_i++)
        {
            fprintf(file_p, " >");
            for (int prev_i = 0; prev_i < nb_nodes_prev; prev_i++)
                fprintf(file_p, " %f",
                        *(*(*(weights_ppp + layer_i) + node_i) + prev_i));
        }
    }
}


int get_nb_layers()
{
    FILE* file_p = fopen(CONFIG_FILE, 'r');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    int nb = 0;

    // skip first line and first char `l` then retrieve number of layer as int
    fscanf(file_p, "%*[^\n]\n%*c %i", &nb);

    if (nb)
        return nb;

    fprintf(stderr, "Invalid l value in %s\n.
            Must be a strictly positive integer.", CONFIG_FILE);
    exit(1);
}


int get_nb_nodes(int nb_layers, int* nb_nodes_p)
{
    FILE* file_p = fopen(CONFIG_FILE, 'r');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    // skip first two lines
    char c;
    for (int i = 0; i < 2; i++)
        do { c = fgetc(file_p); } while (c != '\n');

    int nb;
    for (int i = 0; i < nb_layers; i++)
    {
        fscanf(file_p, " %i", &nb);
        *(nb_nodes_p + i) = nb;
    }
}
