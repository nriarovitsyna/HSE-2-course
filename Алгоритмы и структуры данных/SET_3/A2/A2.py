import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os

def download_data_for_param(switch_param):
    random_file = f'random_results({switch_param}).csv'
    reverse_file = f'reverse_results({switch_param}).csv'
    almost_file = f'almost_results({switch_param}).csv'

    results = {}
    if os.path.exists(random_file):
        results['random'] = pd.read_csv(random_file)
    if os.path.exists(reverse_file):
        results['reverse'] = pd.read_csv(reverse_file)
    if os.path.exists(almost_file):
        results['almost'] = pd.read_csv(almost_file)

    return results, switch_param

def download_all_data(switch_params):
    all_results = {}
    for param in switch_params:
        results, _ = download_data_for_param(param)
        all_results[param] = results
    return all_results

def create_plots_for_param(results, switch_param):
    plot_figure = plt.figure(figsize=(18, 5))
    plot_array = plot_figure.subplots(1, 3)

    plot_figure.suptitle(f'Сравнение производительности Merge Sort и Merge+Insertion Sort (Параметр переключения = {switch_param})',
                        fontsize=16, fontweight='bold')

    plot_types = [
        ('random', 'Случайные массивы'),
        ('reverse', 'Обратно отсортированные массивы'),
        ('almost', 'Почти отсортированные массивы')
    ]

    for i, (data_type, title) in enumerate(plot_types):
        if data_type in results:
            data = results[data_type]
            plot_array[i].plot(data['Size'], data['Merge'],
                             label='Merge Sort', linewidth=2, color='blue')
            plot_array[i].plot(data['Size'], data['MergeInsertion'],
                             label='Merge+Insertion', linewidth=2, color='red')
            plot_array[i].set_title(f'{title}', fontsize=14, fontweight='bold')
            plot_array[i].set_xlabel('Размер массива', fontsize=12)
            plot_array[i].set_ylabel('Время (микросекунды)', fontsize=12)
            plot_array[i].legend(fontsize=10)
            plot_array[i].grid(True, alpha=0.3)

    plt.tight_layout()

    filename = f'results({switch_param}).png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    plt.show()

def create_comparison_plot(all_results, switch_params):
    plt.figure(figsize=(15, 10))

    colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown']
    plot_types = ['random', 'reverse', 'almost']
    titles = ['Случайные массивы', 'Обратно отсортированные массивы', 'Почти отсортированные массивы']

    for plot_idx, (data_type, title) in enumerate(zip(plot_types, titles)):
        plt.subplot(2, 2, plot_idx + 1)

        for i, param in enumerate(switch_params):
            if param in all_results and data_type in all_results[param]:
                data = all_results[param][data_type]
                plt.plot(data['Size'], data['MergeInsertion'],
                        label=f'Param={param}', linewidth=2, color=colors[i % len(colors)])

        plt.title(f'{title}', fontsize=12, fontweight='bold')
        plt.xlabel('Размер массива', fontsize=10)
        plt.ylabel('Время (микросекунды)', fontsize=10)
        plt.legend(fontsize=8)
        plt.grid(True, alpha=0.3)

    plt.suptitle('Сравнение производительности Merge+Insertion Sort для разных параметров переключения',
                fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig('comparison_all_params.png', dpi=300, bbox_inches='tight')
    plt.show()

def main():
    switch_params = [5, 10, 15, 20, 30, 50]

    all_results = download_all_data(switch_params)

    for param in switch_params:
        create_plots_for_param(all_results[param], param)
    create_comparison_plot(all_results, switch_params)

if __name__ == "__main__":
    main()
