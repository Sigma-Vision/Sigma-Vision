#include "../preproc.h"
#include <err.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: neutralize <image-file> <outfile>");
    preproc(argv[1], argv[2]);
}
