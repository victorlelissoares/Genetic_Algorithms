#!/bin/bash
#set -x #seta debug

diretorio="$(pwd)"
diretorio="${diretorio}/benchmarks/$1" # Substitua pelo caminho do diretório desejado
extensao=".vrp"

arquivos_vrp=()  # Cria um array vazio para armazenar os nomes dos arquivos .vrp

# Loop para listar os arquivos com a extensão .vrp no diretório
for arquivo in "$diretorio"/*"$extensao"; do
  arquivos_vrp+=("$arquivo")  # Adiciona o nome do arquivo ao array
done

# Exibe os nomes dos arquivos .vrp armazenados na variável
#echo "Nomes dos arquivos .vrp:"
for arquivo in "${arquivos_vrp[@]}"; do
    echo "Caso de teste: $arquivo"
    #ordem de argumentos ./testAllBenchmarks.sh; diretório de bechmark; número de execuções por instância
   
    namefile=$(basename "$arquivo")
    
    outputfile="${namefile::-4}"
    outputfile="${outputfile}.out"

    #extrai o número de veículos do título do arquivo
    k=$(echo "$namefile" | grep -oP 'k\K\d+' | tr '\n' ' ')

    # Imprima os números encontrados
    echo "Números Veículos: $k"

    #diretório/arquivo onde será escrito o resultado da execução
    resultnamefile="results/${outputfile}"
   
    
    g++ main.cpp -o vrp -O3
    #echo $k
    # Executa n vezes o programa
    for ((i = 1; i <= $2; i++)); do                
        # Inicie o contador de tempo para esta execução
        SECONDS=0

        # Executa o programa e armazena a saída em 'output'
        output=$(./vrp $arquivo $k)

        resultnamefile="results/${outputfile}"
        # echo $resultnamefile

        # Imprima a saída no console e no arquivo de saída
        echo $output | tee -a "$resultnamefile"

        # Calcule o tempo de execução para esta execução do programa
        tempo_execucao=$SECONDS

        # Imprima o tempo de execução no console e no arquivo de saída
        echo "Tempo de execução (segundos): $tempo_execucao" | tee -a "$resultnamefile"
        
        # Sleep de 5 segundos para evitar problemas na geração de números aleatórios
        sleep 5
    done
    rm -f vrp
done