import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 12

files = {
    'Случайный': 'random_results.csv',
    'Обратный': 'reversed_results.csv',
    'Почти отсортированный': 'almost_sorted_results.csv'
}

colors = {
    'StandardQuickSort': 'red',
    'StandardMergeSort': 'blue',
    'StringQuickSort': 'green',
    'StringMergeSort': 'orange',
    'MSDRadixSort': 'purple',
    'MSDRadixSort_QuickSort': 'brown'
}

fig, axes = plt.subplots(3, 2, figsize=(18, 18))

for row, (array_name, filename) in enumerate(files.items()):
    df = pd.read_csv(filename)
    
    ax_time = axes[row, 0]
    for algo in df['Algorithm'].unique():
        algo_data = df[df['Algorithm'] == algo]
        ax_time.plot(algo_data['Size'], algo_data['Time(mcs)'], 
                     marker='o', label=algo, color=colors[algo], linewidth=2)
    ax_time.set_title(f'Время сортировки — {array_name} массив', fontsize=14)
    ax_time.set_xlabel('Размер массива')
    ax_time.set_ylabel('Время (мкс)')
    ax_time.legend(fontsize=10)
    ax_time.grid(True, alpha=0.3)
    
    ax_cmp = axes[row, 1]
    for algo in df['Algorithm'].unique():
        algo_data = df[df['Algorithm'] == algo]
        ax_cmp.plot(algo_data['Size'], algo_data['Comparisons'], 
                    marker='o', label=algo, color=colors[algo], linewidth=2)
    ax_cmp.set_title(f'Количество сравнений — {array_name} массив', fontsize=14)
    ax_cmp.set_xlabel('Размер массива')
    ax_cmp.set_ylabel('Количество сравнений')
    ax_cmp.legend(fontsize=10)
    ax_cmp.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('sorting_analysis.png', dpi=150, bbox_inches='tight')
plt.show()