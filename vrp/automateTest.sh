#!/bin/bash
#set -x #seta debug

g++ main.cpp -o vrp -O3
#ordem de argumentos ./automateTest.sh numExecuções caminho do benchmark a partir da pasta benchmark numCaminhoes

# Executa n vezes o programa
for ((i = 1; i <= $1; i++)); do
    
    #cria arquivo com o nome da instância com extensão out
    namefile="${2:2}"
    namefile="${namefile::-4}"
    namefile="${namefile}.out"
    
    #executando script
    output=$(./vrp benchmarks/$2 $3)

    #imprimindo saída no console e no arquivo de saída
    echo $output | tee -a "results/${namefile}"
    
    #sleep de 5 segundos para não ter problema na geração de números aleatórios
    sleep 5
done
rm -f vrp