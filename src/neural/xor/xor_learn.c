#include <math.h>
#include <stdlib.h>
#include <stdio.h>


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
double dSigmoid(double x)
{
    return x * (1 - x);
}

/**
** Generate random number
*/
double init_weight()
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
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int tmp = array[j];

            array[j] = array[i];
            array[i] = tmp;
        }
    }
}


/**
** Train neural network based on NB_ITER iterations
*/
int learn(const int NB_ITER)
{

    static const int nb_inputs = 2;
    static const int nb_hidden_nodes = 2;
    static const int nb_outputs = 1;

    const double lr = 0.1f;
    double hidden_layer[nb_hidden_nodes];
    double output_layer[nb_outputs];

    double hidden_layer_bias[nb_hidden_nodes];
    double output_layer_bias[nb_outputs];

    double hidden_weights[nb_inputs][nb_hidden_nodes];
    double output_weights[nb_hidden_nodes][nb_outputs];

    static const int nb_training_sets = 4;
    double training_inputs[4][2] =
        { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
    double training_outputs[4][1] = { {0.0f}, {1.0f}, {1.0f}, {0.0f} };

    for (int i = 0; i < nb_inputs; i++)
        for (int j = 0; j < nb_hidden_nodes; j++)
            hidden_weights[i][j] = init_weight();

    for (int i = 0; i < nb_hidden_nodes; i++)
    {
        hidden_layer_bias[i] = init_weight();

        for (int j = 0; j < nb_outputs; j++)
            output_weights[i][j] = init_weight();
    }

    for (int i = 0; i < nb_outputs; i++)
        output_layer_bias[i] = init_weight();

    int training_set_order[] = { 0, 1, 2, 3 };

    for (int n = 0; n < NB_ITER; n++)
    {
        shuffle(training_set_order, nb_training_sets);

        for (int x = 0; x < nb_training_sets; x++)
        {
            int i = training_set_order[x];

            // Forward pass

            for (int j = 0; j < nb_hidden_nodes; j++)
            {
                double activation = hidden_layer_bias[j];

                for (int k = 0; k < nb_inputs; k++)
                    activation += training_inputs[i][k] * hidden_weights[k][j];

                hidden_layer[j] = sigmoid(activation);
            }

            for (int j = 0; j < nb_outputs; j++)
            {
                double activation = output_layer_bias[j];

                for (int k = 0; k < nb_hidden_nodes; k++)
                    activation += hidden_layer[k] * output_weights[k][j];

                output_layer[j] = sigmoid(activation);
            }


            printf("Input: %f %f", training_inputs[i][0],
                                   training_inputs[i][1]);
            printf("    Output: %f", output_layer[0]);
            printf("    Expected Output: %f\n", training_outputs[i][0]);


            // Backprop

            double delta_output[nb_outputs];

            for (int j = 0; j < nb_outputs; j++)
            {
                double errorOutput =
                    (training_outputs[i][j] - output_layer[j]);
                delta_output[j] = errorOutput * dSigmoid(output_layer[j]);
            }

            double delta_hidden[nb_hidden_nodes];

            for (int j = 0; j < nb_hidden_nodes; j++)
            {
                double error_hidden = 0.0f;

                for (int k = 0; k < nb_outputs; k++)
                    error_hidden += delta_output[k] * output_weights[j][k];

                delta_hidden[j] = error_hidden * dSigmoid(hidden_layer[j]);
            }

            for (int j = 0; j < nb_outputs; j++)
            {
                output_layer_bias[j] += delta_output[j] * lr;

                for (int k = 0; k < nb_hidden_nodes; k++)
                    output_weights[k][j] +=
                        hidden_layer[k] * delta_output[j] * lr;
            }

            for (int j = 0; j < nb_hidden_nodes; j++)
            {
                hidden_layer_bias[j] += delta_hidden[j] * lr;

                for (int k = 0; k < nb_inputs; k++)
                    hidden_weights[k][j] +=
                        training_inputs[i][k] * delta_hidden[j] * lr;
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
