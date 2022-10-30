#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double dSigmoid(double x) { return x * (1 - x); }
double init_weight() { return ((double)rand())/((double)RAND_MAX); }
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int learn(const int NB_ITER)
{

    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;

    const double lr = 0.1f;
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];

    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];

    static const int numTrainingSets = 4;
    double training_inputs[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
    double training_outputs[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

    for (int i=0; i<numInputs; i++) {
        for (int j=0; j<numHiddenNodes; j++) {
            hiddenWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numHiddenNodes; i++) {
        hiddenLayerBias[i] = init_weight();
        for (int j=0; j<numOutputs; j++) {
            outputWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numOutputs; i++) {
        outputLayerBias[i] = init_weight();
    }

    int trainingSetOrder[] = {0,1,2,3};

    for (int n=0; n < NB_ITER; n++) {
        shuffle(trainingSetOrder,numTrainingSets);

        for (int x=0; x<numTrainingSets; x++) {

            int i = trainingSetOrder[x];

            // Forward pass

            for (int j=0; j<numHiddenNodes; j++) {
                double activation=hiddenLayerBias[j];
                 for (int k=0; k<numInputs; k++) {
                    activation+=training_inputs[i][k]*hiddenWeights[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }

            for (int j=0; j<numOutputs; j++) {
                double activation=outputLayerBias[j];
                for (int k=0; k<numHiddenNodes; k++) {
                    activation+=hiddenLayer[k]*outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }

            printf("Input: %f %f", training_inputs[i][0], training_inputs[i][1]);
            printf("    Output: %f", outputLayer[0]);
            printf("    Expected Output: %f\n", training_outputs[i][0]);

           // Backprop

            double deltaOutput[numOutputs];
            for (int j=0; j<numOutputs; j++) {
                double errorOutput = (training_outputs[i][j]-outputLayer[j]);
                deltaOutput[j] = errorOutput*dSigmoid(outputLayer[j]);
            }

            double deltaHidden[numHiddenNodes];
            for (int j=0; j<numHiddenNodes; j++) {
                double errorHidden = 0.0f;
                for(int k=0; k<numOutputs; k++) {
                    errorHidden+=deltaOutput[k]*outputWeights[j][k];
                }
                deltaHidden[j] = errorHidden*dSigmoid(hiddenLayer[j]);
            }

            for (int j=0; j<numOutputs; j++) {
                outputLayerBias[j] += deltaOutput[j]*lr;
                for (int k=0; k<numHiddenNodes; k++) {
                    outputWeights[k][j]+=hiddenLayer[k]*deltaOutput[j]*lr;
                }
            }

            for (int j=0; j<numHiddenNodes; j++) {
                hiddenLayerBias[j] += deltaHidden[j]*lr;
                for(int k=0; k<numInputs; k++) {
                    hiddenWeights[k][j]+=training_inputs[i][k]*deltaHidden[j]*lr;
                }
            }
        }

        printf("\n");
    }

    // Print weights
    printf("Final Hidden Weights\n[ ");
    for (int j=0; j<numHiddenNodes; j++) {
        printf("[");
        for(int k=0; k<numInputs; k++) {
            printf(" %f ", hiddenWeights[k][j]);
        }
        printf("] ");
    }
    printf("]\n");

    printf("Final Hidden Biases\n[");
    for (int j=0; j<numHiddenNodes; j++) {
        printf(" %f ", hiddenLayerBias[j]);

    }
    printf("]\n");
    printf("Final Output Weights\n");
    for (int j=0; j<numOutputs; j++) {
        printf("[");
        for (int k=0; k<numHiddenNodes; k++) {
            printf(" %f ", outputWeights[k][j]);
        }
        printf("]\n");
    }
    printf("Final Output Biases\n[");
    for (int j=0; j<numOutputs; j++) {
        printf(" %f ", outputLayerBias[j]);
    }
    printf("]\n");

    return 0;
}
