#include "config_manager.h"

void test_write()
{
    const int nb_inputs = 2;
    const int nb_layers = 3;
    const int* nb_nodes_p = {2, 3, 1};
    const double* biases_p[] = {{1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0}};
    const double* weights_ppp = malloc(nb_layers * size_of(double**));


    int prev_nb_nodes = nb_inputs;
    int nb_nodes;
    for (int layer_i = 0; layer_i < nb_layers; i++)
    {
        nb_nodes = *(nb_nodes_p + layer_i);

        double* weights_pp = malloc(nb_nodes * size_of(double*));
        *(weights_ppp + layer_i) = weights_pp;

        int mem_to_alloc = nb_prev_nodes * size_of(double);

        for (int node_i = 0; node_i < nb_nodes; node_i++)
            *(weights_pp + node_i) = malloc(mem_to_alloc);

        prev_nb_nodes = nb_nodes;
    }

    write_config(nb_inputs, nb_layers, nb_nodes_p, biases_p, );

    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        nb_nodes = *(nb_nodes_p + layer_i);
        weights_pp = *(weights_ppp + layer_i)
        for (int node_i = 0; node_i < nb_nodes; node_i++)
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
