#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include "xor.h"

int main(int argc, char** argv)
{
  if (argc < 2)
    errx(EXIT_FAILURE, "missing parameters");

  if (strcmp(argv[1], "-l") == 0)
  {
    if (argc == 2)
      learn(10000);
    else if (argc == 3)
      learn(atoi(argv[2]));
    else
      errx(EXIT_FAILURE, "too many parameters");
  }
  else if (strcmp(argv[1], "-u") == 0)
  {
    if (argc != 4)
        errx(EXIT_FAILURE, "missing parameters");
    double inputs[] = { (double)atoi(argv[2]), (double)atoi(argv[3]) };
    if (inputs[0] < 0 || inputs[0] > 1 ||
        inputs[1] < 0 || inputs[1] > 1)
        errx(EXIT_FAILURE, "xor input must be 0 or 1");
    use(inputs, 2);
  }
  else
    errx(EXIT_FAILURE, "unknown parameter %s", argv[1]);

  return 0;
}
