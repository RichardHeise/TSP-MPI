#!/bin/python3

import matplotlib.pyplot as plt
import sys

# Lê os dados da entrada padrão
dados = sys.stdin.read()

# Separa os dados em linhas e separe cada linha em campos usando o ponto e vírgula como separador
linhas = dados.strip().split('\n')
cidades = []
medias = []
desvios_padrao = []

# Ignora a primeira linha (cabeçalho)
linhas = linhas[1:]

for linha in linhas:
    campos = linha.split(';')
    if len(campos) == 3:
        num_cidades, media, desvio_padrao = campos
        cidades.append(int(num_cidades))
        medias.append(float(media))
        desvios_padrao.append(float(desvio_padrao))

# Cria o gráfico de linhas com média e desvio padrão
plt.figure(figsize=(10, 6))
plt.plot(cidades, medias, marker='o', label='Média')
plt.errorbar(cidades, medias, yerr=desvios_padrao, fmt='o', capsize=5, label='Desvio Padrão')
plt.xlabel('Número de Cidades')
plt.ylabel('Tempo de Execução')
plt.title('Sequencial')
plt.legend()
plt.grid(True)

# Defina os intervalos inteiros no eixo X
plt.xticks(range(min(cidades), max(cidades) + 1))

# Salva o gráfico em um arquivo PNG
plt.savefig('grafico_tempo_vs_cidades.png')

# Mostra o gráfico (opcional)
# plt.show()

print("Gráfico salvo em grafico_tempo_vs_cidades.png")
