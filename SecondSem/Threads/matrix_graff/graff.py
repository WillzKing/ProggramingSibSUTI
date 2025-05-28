import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


def read_benchmark_results(filename):
    """Чтение файла с результатами бенчмарка"""
    return pd.read_csv(filename, sep='\t')


def plot_benchmark_results(data):
    """Построение графиков по данным бенчмарка"""
    plt.figure(figsize=(12, 8))
    sns.set_style("whitegrid")

    # График времени выполнения для разного числа потоков
    plt.subplot(2, 1, 1)
    for threads in sorted(data['Threads'].unique()):
        subset = data[data['Threads'] == threads]
        plt.plot(subset['Size'], subset['Time(ms)'],
                 marker='o',
                 linestyle='-',
                 linewidth=2,
                 markersize=5,
                 label=f'{threads} потоков')

    plt.title('Производительность умножения матриц', fontsize=14)
    plt.xlabel('Размер матрицы', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.legend(title='Количество потоков', fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.7)

    # График ускорения (speedup)
    plt.subplot(2, 1, 2)
    single_thread = data[data['Threads'] == 1].set_index('Size')['Time(ms)']

    for threads in sorted(data['Threads'].unique()):
        if threads == 1:
            continue
        subset = data[data['Threads'] == threads].set_index('Size')
        speedup = single_thread / subset['Time(ms)']
        plt.plot(speedup.index, speedup.values,
                 marker='s',
                 linestyle='--',
                 linewidth=2,
                 markersize=5,
                 label=f'{threads} потоков')

    plt.title('Ускорение (Speedup) по сравнению с 1 потоком', fontsize=14)
    plt.xlabel('Размер матрицы', fontsize=12)
    plt.ylabel('Коэффициент ускорения', fontsize=12)
    plt.legend(title='Количество потоков', fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.7)

    plt.tight_layout()
    plt.savefig('matrix_multiplication_benchmark.png', dpi=300)
    plt.show()


def main():
    try:
        # Чтение данных из файла
        data = read_benchmark_results('benchmark_results.txt')

        # Построение графиков
        plot_benchmark_results(data)

        print("Графики успешно сохранены в файл 'matrix_multiplication_benchmark.png'")
    except FileNotFoundError:
        print("Ошибка: файл 'benchmark_results.txt' не найден.")
        print("Сначала запустите бенчмарк на C, чтобы создать файл с результатами.")
    except Exception as e:
        print(f"Произошла ошибка: {str(e)}")


if __name__ == "__main__":
    main()