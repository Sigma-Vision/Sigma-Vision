#include <math.h>
#include <stdlib.h>

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
double d_sigmoid(double x)
{   
    return x * (1 - x);
}

/**
** Generate random number
*/
double get_rand_double()
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
