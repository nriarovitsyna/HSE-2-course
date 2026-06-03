import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math

data = pd.read_csv('A1_experiment.csv')

exact_area = 0.25 * math.pi + 1.25 * math.asin(0.8) - 1

plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.plot(data['N'], data['Wide_Area'], 'b-', alpha=0.7, label='Широкая область')
plt.plot(data['N'], data['Narrow_Area'], 'r-', alpha=0.7, label='Узкая область')
plt.axhline(y=exact_area, color='k', linestyle='--', label='Точное значение')
plt.xlabel('Количество точек N')
plt.ylabel('Приближенная площадь')
plt.title('Оценка площади методом Монте-Карло\nВлияние области генерации и объема выборки')
plt.legend()
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(data['N'], data['Wide_Error'], 'b-', alpha=0.7, label='Широкая область')
plt.plot(data['N'], data['Narrow_Error'], 'r-', alpha=0.7, label='Узкая область')
plt.xlabel('Количество точек N')
plt.ylabel('Относительное отклонение')
plt.title('Точность метода Монте-Карло\nЗависимость от области генерации и объема выборки')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('results.png', dpi=300, bbox_inches='tight')
plt.show()
