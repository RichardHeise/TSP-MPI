import matplotlib.pyplot as plt
import numpy as np

def read_data(file_path):
    with open(file_path) as f:
        lines = f.readlines()

    data = {'sequential': {'cores: {'16':[], '17':[], '18':[]}'}, 'parallel': {'cores: {'16':[], '17':[], '18':[]}'}}
    current_test = None

    for line in lines:
        line = line.strip()

        if not line or line == 'ALL DONE':
            continue  # Ignore linhas vazias e a linha final

        if line.startswith('Starting test round...'):
            current_test = None
            continue

        if line.startswith('Sequential data'):
            current_test = 'sequential'
            continue

        if line.startswith('Parallel data'):
            current_test = 'parallel'
            continue

        if line.startswith('16 cities'):
            try:
                cores = int(16)
                current_test += f'_{cores}'
                data[current_test] = {'times': []}
            except (ValueError, IndexError):
                print(f"Ignorando linha inválida: {line}")
                current_test = None
            continue

        try:
            time = float(line)
            data[current_test]['times'].append(time)
        except ValueError:
            print(f"Ignorando linha inválida: {line}")

    return data

def plot_graphs(data):
    for test, values in data.items():
        if 'times' not in values:
            continue

        num_cities = len(values['times'])
        mean_time = np.mean(values['times'])
        std_dev = np.std(values['times'])

        plt.scatter(num_cities, mean_time, label=f'{test} cores')
        plt.errorbar(num_cities, mean_time, yerr=std_dev, linestyle='None')

    plt.xlabel('Number of Cities')
    plt.ylabel('Time')
    plt.title('Test Results')
    plt.legend()
    plt.show()

if __name__ == "__main__":
    file_path = 'results.st'  # Substitua pelo caminho real do seu arquivo
    data = read_data(file_path)
    plot_graphs(data)
