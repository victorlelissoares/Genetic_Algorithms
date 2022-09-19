#!/bin/bash
#benchmarkName num_test
g++ *.cpp -o vrp_simple -Wall -O3 


#executa n vezes o programa
for i in {0..$3-1}
    do
    ./vrp_simple benchmarks/$1 $2 $3 #recebe a saida do programa, que é o fit
done



