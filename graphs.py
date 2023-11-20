import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_and_save_graph(df, num_cores, baseline_data=None):
    grouped_data = df.groupby('cidades')['tempo'].agg(['mean', 'std'])

    plt.errorbar(grouped_data.index, grouped_data['mean'], yerr=grouped_data['std'], label=f'{num_cores} cores',
                 capsize=5, elinewidth=1, marker='o', markersize=5, color='blue')
    
    if baseline_data is not None:
        baseline_grouped = baseline_data.groupby('cidades')['tempo'].agg(['mean', 'std'])
        plt.errorbar(baseline_grouped.index, baseline_grouped['mean'], yerr=baseline_grouped['std'],
                     label='Sequencial', linestyle='-', color='red', capsize=5, elinewidth=1, marker='s', markersize=5)

    plt.xticks(np.arange(min(grouped_data.index), max(grouped_data.index)+1, 1))
    plt.yticks(np.arange(50, 1400, 50))
    plt.xlabel('Número de Cidades')
    plt.ylabel('Tempo de Execução')
    plt.title(f'Tempo de Execução Médio com Desvio Padrão para {num_cores} Cores')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'{file_name}_{num_cores}cores.png')
    plt.clf()  # Limpar a figura para o próximo gráfico

# Leia os arquivos CSV
file_name = 'paralela'
df = pd.read_csv(f'{file_name}.csv', delimiter=';')

baseline_file_name = 'sequencial'
baseline_data = pd.read_csv(f'{baseline_file_name}.csv', delimiter=';')

# Filtrar dados por número de cores
cores_2 = df[df['cores'] == 2]
cores_3 = df[df['cores'] == 3]
cores_4 = df[df['cores'] == 4]
cores_8 = df[df['cores'] == 8]

# Criar gráficos e salvar para cada número de cores
plot_and_save_graph(cores_2, 2, baseline_data)
plot_and_save_graph(cores_3, 3, baseline_data)
plot_and_save_graph(cores_4, 4, baseline_data)
plot_and_save_graph(cores_8, 8, baseline_data)