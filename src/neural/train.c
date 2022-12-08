#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "digit_net.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "missing parameters");

    learn(atoi(argv[1]));

    return 0;
}
