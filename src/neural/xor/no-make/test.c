#include <stdlib.h>
#include "../config_manager.h"

void test_write()
{
    const int nb_inputs = 2;
    const int nb_layers = 3;
    const int nb_nodes[] = {2, 3, 1};
    const int* nb_nodes_p = nb_nodes;
    double** biases_pp = malloc(nb_layers * sizeof(double*));
    double*** weights_ppp = malloc(nb_layers * sizeof(double**));

    int prev_nb_nodes = nb_inputs;
    int curr_nb_nodes;


    /* Allocate memory */
    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        curr_nb_nodes = *(nb_nodes_p + layer_i);

        // Biases
        *(biases_pp + layer_i) = malloc(curr_nb_nodes * sizeof(double));

        // Weights
        double** weights_pp = malloc(curr_nb_nodes * sizeof(double*));
        *(weights_ppp + layer_i) = weights_pp;

        int mem_to_alloc = prev_nb_nodes * sizeof(double);

        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            *(weights_pp + node_i) = malloc(mem_to_alloc);

        prev_nb_nodes = curr_nb_nodes;
    }

    write_config(nb_inputs, nb_layers, nb_nodes_p, biases_pp, weights_ppp);


    /* Free memory */

    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        curr_nb_nodes = *(nb_nodes_p + layer_i);
        double** weights_pp = *(weights_ppp + layer_i);
        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            free(*(weights_pp + node_i));
        free(weights_pp);
    }

    free(weights_ppp);
}


int main()
{
    test_write();

    return 0;
}
