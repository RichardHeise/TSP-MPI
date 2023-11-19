#!/bin/bash

# Função para executar o teste para uma configuração
run_test() {
    local folder="$1"
    local cities="$2"
    local threads="$3"

    echo "$cities cities with $threads" cores >> results_seq.st
    (cd "$folder" && ./run.sh "Relevants/$cities" | grep "Tempo" | cut -d " " -f3) >> results_seq.st
}

echo "Starting test round..." >> results_seq.st

# Configurações sequenciais
echo -e "\nSequential data:" >> results_seq.st

for i in {16..18}; do
    run_test "sequential" "$i" "1"
done

# Configurações paralelas com diferentes valores de OMP_NUM_THREADS
#echo -e "\nParallel data:" >> results.st

#for threads in 2 3 4; do
#    echo -e "\nTesting with OMP_NUM_THREADS=$threads\n" >> results.st

#    for i in {16..18}; do
#        run_test "parallel" "$i" "$threads"
#    done
#done

echo "ALL DONE" >> results_seq.st
