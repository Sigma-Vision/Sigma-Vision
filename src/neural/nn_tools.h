#pragma once

double sigmoid(double x);
double d_sigmoid(double x);

void init_rand();
long get_rand_long(long n);
double get_rand_double();

void shuffle(int* array, size_t n);

void init_params(const int nb_inputs, const int nb_layers, const int nb_nodes[],
                 double** nodes_pp, double** deltas_pp, double** biases_pp,
                 double*** weights_ppp, int learn);

void free_memory(const int nb_layers, const int nb_nodes[], double** nodes_pp,
                 double** deltas_pp, double** biases_pp, double*** weights_ppp);

void get_bit_array(int digit, long n, double bit_a[]);

char get_output(double* outputs);
