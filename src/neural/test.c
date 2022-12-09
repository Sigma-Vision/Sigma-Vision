#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "nn_tools.h"
#include "digit_net.h"


int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "missing parameters");

    if (strcmp(argv[1], "-l") == 0)
    {
        learn(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "-g") == 0)
    {
        init_rand();
        int digit = atoi(argv[2]);
        if (digit < 1 || digit > 9)
            errx(EXIT_FAILURE, "invalid file specifiers");
        long sn = get_rand_long(DATASET_SIZE);
        double* bit_a = malloc(INPUT_SIZE * sizeof(double));
        get_bit_array(digit, sn, bit_a);

        printf("EXPECTED: %i\nRESULT:   %i\n", digit, guess_digit(bit_a));

        free(bit_a);
    }
    else if (strcmp(argv[1], "-t") == 0)
    {
        init_rand();

        int nb_iter = atoi(argv[2]);
        int avg[9] = {0};
        double* bit_a = malloc(INPUT_SIZE * sizeof(double));

        for (int i = 0; i < nb_iter; i++)
        {
            long sn = get_rand_long(DATASET_SIZE);
            for (int digit = 1; digit < 10; digit++)
            {
                get_bit_array(digit, sn, bit_a);
                int res = guess_digit(bit_a);
                if (res == digit)
                    avg[digit]++;
            }
        }

        printf("== STATS ==\n");
        int global = 0;
        for (int i = 0; i < 9; i++)
        {
            global += avg[i];
            printf("%i:  %f\n", i, (float) avg[i] / nb_iter * 100);
        }

        printf("\nAverage: %f\n", (float) global / (9 * nb_iter) * 100);
        free(bit_a);
    }
    else
        errx(EXIT_FAILURE, "unknown parameter %s", argv[1]);

    return 0;
}
