# XOR


## Basic XOR neural network

Adaptation of following cpp code:

> [blog post](https://towardsdatascience.com/simple-neural-network-implementation-in-c-663f51447547)

> [gist](https://gist.github.com/espiritusanti/b7485c68a06ef2c8c76d8c62c8c39d8f)

Created by Santiago Becerra on 9/15/19.


- single hidden layer with 2 neurons
- 2 inputs
- 1 output


### xor.c

Main file to use XOR program with two modes:
- Learn: `./xor -l <nb_iter>`, nb\_iter: number of iteration of work loop,
    writes in stdout weights and biases computed
- Use: `./xor -u <a> <b>`, a & b: input bits (0 or 1), writes in stdout result


### test.c

Test file, ==TODO==


### xor.h

Header for xor\_learn.c and xor\_use.c


### xor\_use.c

Exploits trained neural network and returns computed result


### xor\_learn.c

Trains neural network from scratch
