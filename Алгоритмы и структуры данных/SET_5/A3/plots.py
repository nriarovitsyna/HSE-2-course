import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.read_csv('results.csv')

print("=== Обоснование параметра B ===")
print(f"B = 12, m = 2^12 = {2**12} регистров")
print(f"Память: {2**12} * 1 байт = {2**12 / 1024:.1f} KB")
print(f"Теоретическая ошибка (1.04/√m): {1.04/np.sqrt(2**12):.4f} ({1.04/np.sqrt(2**12)*100:.2f}%)")
print(f"Теоретическая ошибка (1.3/√m): {1.3/np.sqrt(2**12):.4f} ({1.3/np.sqrt(2**12)*100:.2f}%)")
print()

steps = []
exact_mean = []
est_mean = []
sigma = []
relative_errors = []

for p in sorted(df['percentage'].unique()):
    subset = df[df['percentage'] == p]
    steps.append(p)
    exact_mean.append(subset['exact'].mean())
    est_mean.append(subset['estimate'].mean())
    sigma.append(subset['estimate'].std())
    
    rel_err = np.abs(subset['estimate'].mean() - subset['exact'].mean()) / subset['exact'].mean() * 100
    relative_errors.append(rel_err)

steps = np.array(steps)
exact_mean = np.array(exact_mean)
est_mean = np.array(est_mean)
sigma = np.array(sigma)
relative_errors = np.array(relative_errors)

plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.plot(steps, exact_mean, label='F0^t (точное)', marker='o', linewidth=2)
plt.plot(steps, est_mean, label='Nt (HyperLogLog)', marker='s', linewidth=2)
plt.xlabel('Обработанная часть потока (%)')
plt.ylabel('Уникальные элементы')
plt.title('Сравнение оценки Nt и F0^t')
plt.legend()
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(steps, relative_errors, marker='o', linewidth=2, color='purple', label='Фактическая ошибка')
plt.axhline(y=1.04/np.sqrt(2**12)*100, color='green', linestyle='--', label='1.04/√m')
plt.axhline(y=1.3/np.sqrt(2**12)*100, color='red', linestyle='--', label='1.3/√m')
plt.fill_between([0, 100], 0, 1.3/np.sqrt(2**12)*100, alpha=0.1, color='red')
plt.xlabel('Обработанная часть потока (%)')
plt.ylabel('Относительная ошибка (%)')
plt.title('Точность HyperLogLog vs теоретические границы')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(steps, est_mean, label='E(Nt)', color='blue', linewidth=2)
plt.fill_between(steps, est_mean - sigma, est_mean + sigma, alpha=0.3, label='±σNt', color='blue')
plt.xlabel('Обработанная часть потока (%)')
plt.ylabel('Уникальные элементы')
plt.title('Статистики оценки: среднее и стандартное отклонение')
plt.legend()
plt.grid(True)
plt.show()

print("=== Статистики точности ===")
print(f"Средняя относительная ошибка: {np.mean(relative_errors):.2f}%")
print(f"Максимальная ошибка: {np.max(relative_errors):.2f}%")
print(f"Стандартное отклонение оценок: {np.mean(sigma):.2f} элементов")
print()

within_small = np.sum(relative_errors <= 1.04/np.sqrt(2**12)*100)
within_large = np.sum(relative_errors <= 1.3/np.sqrt(2**12)*100)
print("=== Проверка теоретических границ ===")
print(f"Ошибка ≤ 1.04/√m в {within_small}/{len(steps)} случаях ({within_small/len(steps)*100:.1f}%)")
print(f"Ошибка ≤ 1.3/√m в {within_large}/{len(steps)} случаях ({within_large/len(steps)*100:.1f}%)")