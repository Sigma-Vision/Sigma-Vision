#pragma once
#define PICTURE_WIDTH 28
#define INPUT_SIZE 784
#define DATASET_SIZE 500   //number of training picture per digit

int learn(int NB_ITER);
int guess_digit(double inputs[]);
