# NNCONF file format guide

> Neural Network CONFiguration

## Global

- .nnconf files are written in plain text
- Data is stored line by line
- Each line starts with a single character: the **tag**
- Values are given in descending order

## Tags

- `#`   Comment (ignored during reading)
- `l`   Number of hidden layers
- `n`   Number of neurons (or inputs in INPUT section)
- `b`   Biases values
- `w`   Weights values

## Exemple

```
# START
l 1
# INPUT
n 2
# HIDDEN
n 2
b 1 1.5
w > 0.2 3.12 > 4.2 6.9
# OUTPUT
n 1
b 0.77
w > 1.23 3.21
```

This file stores a neural network with 2 inputs, 1 hidden layer of 2 neurons and
1 output layer with a single neuron.

```
IN  HIDDEN

 > ---()
   \ /  \
    X    () > OUT
   / \  /
 > ---()

```
