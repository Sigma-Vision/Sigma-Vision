#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "nn_tools.h"
#include "config_manager.h"


/**
** Train neural network based on NB_ITER iterations
*/
int learn(const int NB_ITER)
{
    /* Network configuration */
    const int nb_inputs = 2;
    const int nb_layers = 2;
    const int nb_nodes[] = {3, 2};

    double** nodes_pp = malloc(nb_layers * sizeof(double*));
    double** deltas_pp = malloc(nb_layers * sizeof(double*));
    double** biases_pp = malloc(nb_layers * sizeof(double*));
    double*** weights_ppp = malloc(nb_layers * sizeof(double**));

    init_params(nb_inputs, nb_layers, nb_nodes, nodes_pp, deltas_pp,
                biases_pp, weights_ppp, 1);


    /* Training set */

    static const int nb_training_sets = 4;
    double training_inputs[4][2] =
           { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
    double training_outputs[4][2] =
           { {1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f} };

    int training_set_order[] = { 0, 1, 2, 3 };


    const double learning_rate = 0.1f;


    /* TRAINING LOOP */

    for (int iter_i = 0; iter_i < NB_ITER; iter_i++)
    {
        shuffle(training_set_order, nb_training_sets);

        for (int x = 0; x < nb_training_sets; x++)
        {
            int i = training_set_order[x];
            double* nodes_p = *nodes_pp;
            double* biases_p = *biases_pp;
            double** weights_pp = *weights_ppp;


            /* FORWARD PASS */

            int prev_nb_nodes = nb_inputs;
            int curr_nb_nodes = nb_nodes[0];

            // input layer
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            {
                double activation = *(biases_p + node_i);
                double* weights_p = *(weights_pp + node_i);

                for (int input_i = 0; input_i < nb_inputs; input_i++)
                    activation += training_inputs[i][input_i]
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


            /*
            printf("Input: %f %f", training_inputs[i][0],
                                   training_inputs[i][1]);
            printf("    Output: %f %f", *(*(nodes_pp + nb_layers - 1))
                                      , *(*(nodes_pp + nb_layers - 1) + 1));
            printf("    Expected Output: %f %f\n", training_outputs[i][0]
                                                 , training_outputs[i][1]);
            */


            /* BACK PROPAGATION */

            /* Get deltas */
            prev_nb_nodes = nb_nodes[nb_layers - 1];
            curr_nb_nodes = prev_nb_nodes;
            nodes_p = *(nodes_pp + nb_layers - 1);
            double* prev_deltas_p;
            double* curr_deltas_p = *(deltas_pp + nb_layers - 1);

            // output layer
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
                *(curr_deltas_p + node_i) = d_sigmoid(*(nodes_p + node_i))
                    * (training_outputs[i][node_i] - *(nodes_p + node_i));

            for (int layer_i = nb_layers - 2; layer_i >= 0; layer_i--)
            {
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
                        training_inputs[i][input_i] * factor;
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


    write_config(nb_inputs, nb_layers, nb_nodes, biases_pp, weights_ppp);

    free_memory(nb_layers, nb_nodes, nodes_pp, deltas_pp, biases_pp,
                weights_ppp);

    return 0;
}
