#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "nn_tools.h"
#include "config_manager.h"

void init_params(const int nb_inputs, const int nb_layers, int* nb_nodes_p,
                 double** nodes_pp, double** biases_pp, double*** weights_ppp)
{
    int prev_nb_nodes = nb_inputs;
    int curr_nb_nodes;

    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        curr_nb_nodes = *(nb_nodes_p + layer_i);

        int layer_mem_size = curr_nb_nodes * sizeof(double);

        *(nodes_pp + layer_i) = calloc(layer_mem_size);
        double* biases_p = malloc(layer_mem_size);
        double** weights_pp = malloc(layer_mem_size);
        *(biases_pp + layer_i) = biases_p;
        *(weights_ppp + layer_i) = weights_pp;

        int node_mem_size = prev_nb_nodes * sizeof(double);
        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
        {
            *(biases_p + node_i) = get_rand_double();

            double* weights_p = malloc(node_mem_size);
            *(weights_pp + node_i) = weights_p;

            for (int prev_node_i = 0; prev_node_i < prev_nb_nodes; prev_node_i++)
                *(weights_p + prev_node_i) = get_rand_double;
        }

        prev_nb_nodes = curr_nb_nodes;
    }
}

void free_memory(const int nb_layers, int* nb_nodes_p, double** nodes_pp,
                 double** biases_pp, double*** weights_ppp)
{
    for (int layer_i = 0; layer_i < nb_layers; layer_i++)
    {
        curr_nb_nodes = *(nb_nodes_p + layer_i);

        // Nodes
        double* nodes_p = *(nodes_pp + layer_i);
        free(nodes_p);

        // Biases
        double* biases_p = *(biases_pp + layer_i);
        free(biases_p);

        // Weights
        double** weights_pp = *(weights_ppp + layer_i);
        for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            free(*(weights_pp + node_i));
        free(weights_pp);
    }

    free(biases_pp);
    free(weights_ppp);
}

/**
** Train neural network based on NB_ITER iterations
*/
int learn(const int NB_ITER)
{
    /* Network configuration */
    const int nb_inputs = 2;
    const int nb_layers = 2;
    const int nb_nodes[] = {2, 1};
    const int* nb_nodes_p = nb_nodes;

    double** nodes_pp = malloc(nb_layers * sizeof(double*));
    double** biases_pp = malloc(nb_layers * sizeof(double*));
    double*** weights_ppp = malloc(nb_layers * sizeof(double**));

    init_params(nb_inputs, nb_layers, nb_nodes_p, nodes_pp, biases_pp, weights_ppp);


    /* Training set */

    static const int nb_training_sets = 4;
    double training_inputs[nb_training_sets][2] =
           { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
    double training_outputs[nb_training_sets][1] =
           { {0.0f}, {1.0f}, {1.0f}, {0.0f} };

    int training_set_order[] = { 0, 1, 2, 3 };


    const double learning_rate = 0.1f;


    /* TRAINING LOOP */

    for (int iter_i = 0; iter_i < NB_ITER; iter_i++)
    {
        shuffle(training_set_order, nb_training_sets);

        for (int x = 0; x < nb_training_sets; x++)
        {
            int i = training_set_order[x];

            /* Forward pass */

            int prev_nb_nodes = nb_inputs;
            int curr_nb_nodes = nb_nodes[0];
            double* nodes_p = *nodes_pp;
            double* biases_p = *biases_pp;
            double** weights_pp = *weights_pp;

            // input layer
            for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
            {
                double activation = *(biases_p + node_i);
                double* weights_p = *(weights_pp + node_i);

                for (int prev_node_i; prev_node_i < prev_nb_nodes; prev_node_i++)
                    activation += *(prev_nodes_p + prev_node_i)
                                * *(weights_p + prev_node_i);

                *(nodes_p + node_i) = sigmoid(activation);
            }

            for (int layer_i = 1; layer_i < nb_layers; layer_i++)
            {
                curr_nb_nodes = *(nb_nodes_p + layer_i);
                double* prev_nodes_p = *(nodes_pp + layer_i - 1);
                nodes_p = *(nodes_pp + layer_i);
                biases_p = *(biases_pp + layer_i);
                weights_pp = *(weights_pp + layer_i);

                for (int node_i = 0; node_i < curr_nb_nodes; node_i++)
                {
                    double activation = *(biases_p + node_i);
                    double* weights_p = *(weights_pp + node_i);

                    for (int prev_node_i; prev_node_i < prev_nb_nodes; prev_node_i++)
                        activation += *(prev_nodes_p + prev_node_i)
                                    * *(weights_p + prev_node_i);

                    *(nodes_p + node_i) = sigmoid(activation);
                }
            }


            printf("Input: %f %f", training_inputs[i][0],
                                   training_inputs[i][1]);
            printf("    Output: %f", *(*(nodes_pp + nb_layers - 1)));
            printf("    Expected Output: %f\n", training_outputs[i][0]);


            // Backprop

            double delta_output[nb_outputs];

            for (int j = 0; j < nb_outputs; j++)
            {
                double errorOutput =
                    (training_outputs[i][j] - output_layer[j]);
                delta_output[j] = errorOutput * d_sigmoid(output_layer[j]);
            }

            double delta_hidden[nb_hidden_nodes];

            for (int j = 0; j < nb_hidden_nodes; j++)
            {
                double error_hidden = 0.0f;

                for (int k = 0; k < nb_outputs; k++)
                    error_hidden += delta_output[k] * output_weights[j][k];

                delta_hidden[j] = error_hidden * d_sigmoid(hidden_layer[j]);
            }

            for (int j = 0; j < nb_outputs; j++)
            {
                output_layer_bias[j] += delta_output[j] * learning_rate;

                for (int k = 0; k < nb_hidden_nodes; k++)
                    output_weights[k][j] +=
                        hidden_layer[k] * delta_output[j] * learning_rate;
            }

            for (int j = 0; j < nb_hidden_nodes; j++)
            {
                hidden_layer_bias[j] += delta_hidden[j] * learning_rate;

                for (int k = 0; k < nb_inputs; k++)
                    hidden_weights[k][j] +=
                        training_inputs[i][k] * delta_hidden[j] * learning_rate;
            }
        }

        printf("\n");
    }


    // Print weights

    printf("Final Hidden Weights\n[ ");
    for (int j = 0; j < nb_hidden_nodes; j++)
    {
        printf("[");
        for (int k = 0; k < nb_inputs; k++)
            printf(" %f ", hidden_weights[k][j]);
        printf("] ");
    }
    printf("]\n");

    printf("Final Hidden Biases\n[");
    for (int j = 0; j < nb_hidden_nodes; j++)
        printf(" %f ", hidden_layer_bias[j]);
    printf("]\n");

    printf("Final Output Weights\n");
    for (int j = 0; j < nb_outputs; j++)
    {
        printf("[");
        for (int k = 0; k < nb_hidden_nodes; k++)
            printf(" %f ", output_weights[k][j]);
        printf("]\n");
    }

    printf("Final Output Biases\n[");
    for (int j = 0; j < nb_outputs; j++)
        printf(" %f ", output_layer_bias[j]);

    printf("]\n");

    return 0;
}
