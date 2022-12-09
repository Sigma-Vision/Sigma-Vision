#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "nn_tools.h"
#include "config_manager.h"
#include "digit_net.h"


/**
** Train neural network based on NB_ITER iterations
*/
int learn(const int NB_ITER)
{
    //printf("Init params\n");
    init_rand();

    /* Network configuration */
    const int nb_inputs = INPUT_SIZE;
    const int nb_layers = 4;
    const int nb_nodes[] = {392, 98, 49, 9};

    double** nodes_pp = malloc(nb_layers * sizeof(double*));
    double** deltas_pp = malloc(nb_layers * sizeof(double*));
    double** biases_pp = malloc(nb_layers * sizeof(double*));
    double*** weights_ppp = malloc(nb_layers * sizeof(double**));

    init_params(nb_inputs, nb_layers, nb_nodes, nodes_pp, deltas_pp,
                biases_pp, weights_ppp, 1);

    /* Training set */
    static const int nb_digits = 9;
    int digit_set_order[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8};
    double training_inputs[INPUT_SIZE] = {0};

    const double learning_rate = 0.0001f;


    /* TRAINING LOOP */

    for (int iter_i = 0; iter_i < NB_ITER; iter_i++)
    {
        shuffle(digit_set_order, nb_digits);

        long file_id = get_rand_long(DATASET_SIZE);

        for (int x = 0; x < nb_digits; x++)
        {
            int i = digit_set_order[x];
            get_bit_array(i + 1, file_id, training_inputs);

            double* nodes_p = *nodes_pp;
            double* biases_p = *biases_pp;
            double** weights_pp = *weights_ppp;


            /* FORWARD PASS */
            //printf("Forward pass\n");

            int prev_nb_nodes = nb_inputs;
            int curr_nb_nodes = nb_nodes[0];

            // input layer
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            {
                double activation = *(biases_p + node_i);
                double* weights_p = *(weights_pp + node_i);

                for (int input_i = 0; input_i < nb_inputs; input_i++)
                    activation += training_inputs[input_i]
                                * *(weights_p + input_i);

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

                    for (int prev_node_i = 0; prev_node_i < prev_nb_nodes;
                         prev_node_i++)
                        activation += *(prev_nodes_p + prev_node_i)
                                    * *(weights_p + prev_node_i);

                    *(nodes_p + node_i) = sigmoid(activation);
                }
            }


            /* BACK PROPAGATION */

            /* Get deltas */
            //printf("Get deltas\n");

            prev_nb_nodes = nb_nodes[nb_layers - 1];
            curr_nb_nodes = prev_nb_nodes;
            nodes_p = *(nodes_pp + nb_layers - 1);
            double* prev_deltas_p;
            double* curr_deltas_p = *(deltas_pp + nb_layers - 1);

            // output layer
            //printf("--output layer\n");
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
                *(curr_deltas_p + node_i) = -*(nodes_p + node_i);
            *(curr_deltas_p + i) = d_sigmoid(*(nodes_p + i)) - *(nodes_p + i);

            for (int layer_i = nb_layers - 2; layer_i >= 0; layer_i--)
            {
                //printf("--layer %i\n", layer_i);
                prev_nb_nodes = curr_nb_nodes;
                curr_nb_nodes = nb_nodes[layer_i];
                nodes_p = *(nodes_pp + layer_i);
                prev_deltas_p = curr_deltas_p;
                curr_deltas_p = *(deltas_pp + layer_i);
                // next layer weights!
                weights_pp = *(weights_ppp + layer_i + 1);

                for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
                {
                    double error = 0.0f;
                    for (int prev_node_i = 0; prev_node_i < prev_nb_nodes;
                            prev_node_i++)
                        error += *(prev_deltas_p + prev_node_i)
                               * *(*(weights_pp + prev_node_i) + node_i);
                    *(curr_deltas_p + node_i) =
                        error * d_sigmoid(*(nodes_p + node_i));
                }
            }


            /* Adapt biases & weights */
            //printf("Adapt biases & weights\n");

            prev_nb_nodes = nb_inputs;
            curr_nb_nodes = nb_nodes[0];
            double* deltas_p = *deltas_pp;
            nodes_p = *nodes_pp;
            weights_pp = *weights_ppp;

            // input layer
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            {
                double factor = *(deltas_p + node_i) * learning_rate;
                *(*biases_pp + node_i) += factor;

                double* weights_p = *(weights_pp + node_i);
                for (int input_i = 0; input_i < nb_inputs; input_i++)
                    *(weights_p + input_i) +=
                        training_inputs[input_i] * factor;
            }

            for (int layer_i = 1; layer_i < nb_layers; layer_i++)
            {
                prev_nb_nodes = curr_nb_nodes;
                curr_nb_nodes = nb_nodes[layer_i];

                double* prev_nodes_p = nodes_p;
                nodes_p = *(nodes_pp + layer_i);
                deltas_p = *(deltas_pp + layer_i);
                biases_p = *(biases_pp + layer_i);
                weights_pp = *(weights_ppp + layer_i);

                for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
                {
                    double factor = *(deltas_p + node_i) * learning_rate;

                    *(biases_p + node_i) += factor;

                    double* weights_p = *(weights_pp + node_i);
                    for (int prev_node_i = 0; prev_node_i < prev_nb_nodes;
                         prev_node_i++)
                        *(weights_p + prev_node_i) +=
                            *(prev_nodes_p + prev_node_i) * factor;
                }
            }
        }

        //printf("\n");
    }


    //printf("Write config\n");
    write_config(nb_inputs, nb_layers, nb_nodes, biases_pp, weights_ppp);

    free_memory(nb_layers, nb_nodes, nodes_pp, deltas_pp, biases_pp,
                weights_ppp);

    return 0;
}
