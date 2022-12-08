#pragma once
#define PICTURE_WIDTH 16
#define INPUT_SIZE 256
#define DATASET_SIZE 10000   //number of training picture per digit

int learn(int NB_ITER);
int guess(double inputs[], int nb_inputs);
