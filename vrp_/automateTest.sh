#!/bin/bash
#benchmarkName num_test
g++ *.cpp -o vrp_simple -Wall -O3 
value = '0'

#executa n vezes o programa
for i in {0..($2-1)}
    do
    value = value + './vrp_simple benchmarks/$1'#recebe a saida do programa, que é o fit
done

#faz a média das distâncias
value = value / $2

echo "Distância média percorrida: $value\n"


