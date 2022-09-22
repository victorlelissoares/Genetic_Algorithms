#!/bin/bash

g++ *.cpp -o vrp_simple -O3 


#executa n vezes o programa
for (( i = 1; i <= $1; i++))
    do
    value_obtined = $(./vrp_simple benchmarks/$2 $3) #recebe a saida do programa, que é o fit
    
    gap = $[[100 * [$4 - $value_obtined] / $4] + $gap]
done





