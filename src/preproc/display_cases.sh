#!/bin/bash

for i in {0..8}
do
    for j in {0..8}
    do
        echo "row ${i} : column ${j}"
        timeout 2 feh "r${i}_c${j}.case" 
    done
done
