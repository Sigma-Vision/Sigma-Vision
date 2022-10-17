#include "../solver.h"
#include <err.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
        solve_file(argv[1]);
    else
        errx(1, "no file specified");
    return 0;
}
