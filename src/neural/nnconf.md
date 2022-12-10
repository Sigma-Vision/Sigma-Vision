# NNCONF file format guide

> Neural Network CONFiguration

## Global

- .nnconf files are written in plain text
- Data is stored line by line
- Each line starts with a single character: the **tag**
- Values are given in descending order

## Tags

- `#`   Comment (ignored during reading)
- `l`   Number of hidden layers `l number`
- `s`   Node number summary     `s inputs layer_1 [layer_2] [...] output`
- `b`   Biases values           `b bias_1 [bias_2] [...]`
- `w`   Weights values          `w > weight_1_1 [...] [> weight_2_1 [...]] [...]`

## Exemple

```
# SUMMARY
l 3
s 2 2 3 1
# DATA
b 1 1
w > 1 1 > 1 1
b 1 1 1
w > 1 1 > 1 1 > 1 1
b 1
w > 1 1 1
```

This file stores a neural network with 2 inputs, 2 hidden layers with respectively
2 and 3 nodes and 1 output layer with a single node.

In this example every biases and weights are set to 1.

--

```
IN |    HIDDEN    | OUT

  >-     (b)---(d)
    \   /   \ /   \
     (a)     X     (f)->
    /   \   / \   /
  >-     (c)---(e)
```

In this example, order in file is `(a)-> (b -> c) -> (d -> e) -> (f)`
