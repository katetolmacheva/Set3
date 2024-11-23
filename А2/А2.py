import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных из CSV-файла
data = pd.read_csv('results.csv')

# Функция для построения графиков для каждого типа массива
def plot_sort_times(array_type):
    subset = data[data['Type'] == array_type]
    plt.figure(figsize=(10,6))
    for algorithm in subset['Algorithm'].unique():
        alg_data = subset[subset['Algorithm'] == algorithm]
        plt.plot(alg_data['Size'], alg_data['Time_microseconds'], label=algorithm)
    plt.title(f"Время сортировки для массива типа '{array_type}'")
    plt.xlabel("Размер массива")
    plt.ylabel("Время сортировки (микросекунды)")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"Sort_Time_{array_type.replace(' ', '_')}.png")
    plt.show()

# Функция для построения графиков относительного отклонения
def plot_sort_relative_error():
    # Вычисление относительного отклонения времени гибридного алгоритма от стандартного
    pivot = data[data['Algorithm'] == 'Standard Merge Sort'][['Size', 'Type', 'Time_microseconds']]
    pivot = pivot.rename(columns={'Time_microseconds': 'Standard_Time'})
    hybrid = data[data['Algorithm'] == 'Hybrid Merge+Insertion Sort'][['Size', 'Type', 'Time_microseconds']]
    merged = pd.merge(pivot, hybrid, on=['Size', 'Type'], suffixes=('_Standard', '_Hybrid'))
    merged['Relative_Error'] = (merged['Time_microseconds_Hybrid'] - merged['Time_microseconds_Standard']) / merged['Time_microseconds_Standard'] * 100

    # Построение графиков для каждого типа массива
    for array_type in merged['Type'].unique():
        subset = merged[merged['Type'] == array_type]
        plt.figure(figsize=(10,6))
        plt.plot(subset['Size'], subset['Relative_Error'], label='Relative Error (%)', color='purple')
        plt.title(f"Относительное отклонение времени сортировки для '{array_type}'")
        plt.xlabel("Размер массива")
        plt.ylabel("Относительное отклонение (%)")
        plt.legend()
        plt.grid(True)
        plt.savefig(f"Relative_Error_{array_type.replace(' ', '_')}.png")
        plt.show()

# Построение графиков времени сортировки
for array_type in data['Type'].unique():
    plot_sort_times(array_type)

# Построение графиков относительного отклонения
plot_sort_relative_error()
