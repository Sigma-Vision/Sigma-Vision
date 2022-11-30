#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "nn_tools.h"
#include "config_manager.h"
#include "digit_net.h"


/**
** Determine most probable expected output
*/
char get_output(double* outputs)
{
    double max_v = 0;
    char res;

    for (char i = 0; i < 9; i++)
        if (*(outputs + i) > max_v)
        {
            max_v = *(outputs + i);
            res = i + 1;
        }
    return res;
}


/**
** Guess digit value present in bit array inputs
*/
int use(double inputs[])
{
    /* Network configuration */
    if (INPUT_SIZE != get_nb_inputs())
    {
        fprintf(stderr, "Invalid number of inputs. %i in config file, %i in header.\n",
                get_nb_inputs, INPUT_SIZE());
        exit(1);
    }

    const int nb_layers = get_nb_layers();
    int nb_nodes[nb_layers];

    double** nodes_pp = malloc(nb_layers * sizeof(double*));
    double** biases_pp = malloc(nb_layers * sizeof(double*));
    double*** weights_ppp = malloc(nb_layers * sizeof(double**));

    get_nb_nodes(nb_layers, nb_nodes);

    init_params(INPUT_SIZE, nb_layers, nb_nodes, nodes_pp, 0,
                biases_pp, weights_ppp, 0);

    get_config(INPUT_SIZE, nb_layers, nb_nodes, biases_pp, weights_ppp);

    double* nodes_p = *nodes_pp;
    double* biases_p = *biases_pp;
    double** weights_pp = *weights_ppp;

    int prev_nb_nodes = INPUT_SIZE;
    int curr_nb_nodes = nb_nodes[0];

    // input layer
    for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
    {
        double activation = *(biases_p + node_i);
        double* weights_p = *(weights_pp + node_i);

        for (int input_i = 0; input_i < INPUT_SIZE; input_i++)
            activation += inputs[input_i] * *(weights_p + input_i);

        *(nodes_p + node_i) = sigmoid(activation);
    }

    for (int layer_i = 1; layer_i < nb_layers; layer_i++)
    {
        prev_nb_nodes = curr_nb_nodes;
        curr_nb_nodes = nb_nodes[layer_i];

        double* prev_nodes_p = *(nodes_pp + layer_i - 1);
        nodes_p = *(nodes_pp + layer_i);
        biases_p = *(biases_pp + layer_i);
        weights_pp = *(weights_ppp + layer_i);

        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
        {
            double activation = *(biases_p + node_i);
            double* weights_p = *(weights_pp + node_i);

            for (int prev_node_i = 0; prev_node_i < prev_nb_nodes; prev_node_i++)
                activation += *(prev_nodes_p + prev_node_i)
                            * *(weights_p + prev_node_i);

            *(nodes_p + node_i) = sigmoid(activation);
        }
    }

    char result = get_output(*(nodes_pp + nb_layers - 1);
    printf("RESULT: %i\n", result);

    free_memory(nb_layers, nb_nodes, nodes_pp, 0, biases_pp,
                weights_ppp);

    return result;
}
