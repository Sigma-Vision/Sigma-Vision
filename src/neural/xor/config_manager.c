#include <stdio.h>
#include <stdlib.h>
#include "config_manager.h"


/** TOOLS **/

// move file pointer to next nb line in file stream file_p
void skip_line(const int nb; FILE* file_p)
{
    char c;
    for (int i = 0; i < nb && fscanf(file_p, "%c") != EOF; i++)
        do { c = fgetc(file_p); } while (c != '\n');
}

// move file pointer to next nb data subblock (separated by space)
// in file stream file_p
void skip_block(const int nb; FILE* file_p)
{
    char c;
    for (int i = 0; i < nb && fscanf(file_p, "%c") != EOF; i++)
        do { c = fgetc(file_p); } while (c != ' ' && c != '\n');
}


/** WRITE **/

void write_config(const int nb_inputs, const int nb_layers,
                  const int* nb_nodes_p, const int** biases_pp,
                  const int*** weights_ppp)
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


/** READ **/

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

    fprintf(stderr, "Invalid `l` value in %s\n.
            Must be a strictly positive integer.\n", CONFIG_FILE);
    exit(1);
}

int get_nb_inputs()
{
    FILE* file_p = fopen(CONFIG_FILE, 'r');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    int nb = 0;

    // skip first two lines and first char `s` then retrieve number of inputs as int
    fscanf(file_p, "%*[^\n]\n%*[^\n]\n%*c %i", &nb);

    if (nb)
        return nb;

    fprintf(stderr, "Invalid first `s` value in %s\n.
            Must be a strictly positive integer.\n", CONFIG_FILE);
    exit(1);
}

void get_nb_nodes(const int nb_layers, int* nb_nodes_p)
{
    FILE* file_p = fopen(CONFIG_FILE, 'r');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    // #SUMMARY and line `l`
    skip_line(2, file_p);
    // tag `s` and nb_inputs
    skip_block(2, file_p);

    int nb;
    for (int i = 0; i < nb_layers; i++)
    {
        fscanf(file_p, " %i", &nb);
        *(nb_nodes_p + i) = nb;
        skip_block(1, file_p);
    }
}

void get_config(const int nb_inputs, const int nb_layers, const int* nb_nodes_p,
                int** biases_pp, int*** weights_ppp)
{
    FILE* file_p = fopen(CONFIG_FILE, 'r');

    if (file_p == NULL)
    {
        fprintf(stderr, "Failed to open config file %s\n", CONFIG_FILE);
        exit(1);
    }

    skip_line(4, file_p);

    double value_buf;
    // number of nodes in previous layer
    int prev_nb_nodes = nb_inputs;

    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        // number of nodes in this layer
        nb_nodes = *(nb_nodes_p + layer_i);


        /* parse biases */

        // check tag
        if (fgetc(file_p) != 'b')
        {
            fprintf(stderr, "Missing tag `b` in %s\n", CONFIG_FILE);
            exit(1);
        }
        // skip space
        fgetc(file_p);

        int* biases_p = *(biases_pp + layer_i);
        for (int node_i = 0; node_i < nb_nodes; node_i++)
        {
            fscanf(file_p, " %f", &value_buf);
            *(biases_p + node_i) = value_buff;
            skip_block(1, file_p);
        }


        /* parse weights */

        // check tag
        if (fgetc(file_p) != 'w')
        {
            fprintf(stderr, "Missing tag `w` in %s\n", CONFIG_FILE);
            exit(1);
        }
        // skip space
        fgetc(file_p);

        int** weights_pp = *(weights_ppp + layer_i);
        for (int node_i = 0; node_i < nb_nodes; node_i++)
        {
            int* weights_p = *(weights_pp + node_i);
            for (int prev_node_i = 0; prev_node_i < prev_node_nb; prev_node_i++)
            {
                // check tag
                if (fgetc(file_p) != '>')
                {
                    fprintf(stderr, "Missing tag `>` in %s\n", CONFIG_FILE);
                    exit(1);
                }
                // skip space
                fgetc(file_p);

                // get value
                fscanf(file_p, " %f", &value_buf);
                *(weights_p + prev_node_i) = value_buff;
                skip_block(1, file_p);
            }
        }
    }
}
