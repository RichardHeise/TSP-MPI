#!/bin/python3

import random
import sys

# Verifica se o número correto de argumentos foi passado na linha de comando
if len(sys.argv) != 3:
    print("Uso: python geratestes.py <N> <M>")
    sys.exit(1)

try:
    N = int(sys.argv[1])
    M = int(sys.argv[2])

    if N <= 0 or M <= 0:
        print("Erro: N e M devem ser números inteiros positivos.")
        sys.exit(1)
    print(N)
    for _ in range(N):
        pairs = set()
        while len(pairs) < M:
            x = random.randint(0, (M * M) - 1)
            y = random.randint(0, (M * M) - 1)
            pairs.add((x, y))

        print(M)
        for x, y in pairs:
            print(f"{x} {y}")

except ValueError:
    print("Erro: Forneça valores inteiros positivos para N e M.")
except Exception as e:
    print(f"Erro inesperado: {str(e)}")
