import math

def main():
    # Тестовые данные для одиночных значений
    test_cases = [1.0, -1.0, 0.0, 0.66, -0.33, 0.99, -0.99]

    # Результаты из ассемблерной программы для одиночных значений
    assembler_results = {
        1.0: 0.0,
        -1.0: 3.1415927,
        0.0: 1.5707964,
        0.66: 0.8499779,
        -0.33: 1.9071,
        0.99: 0.14157665,
        -0.99: 3.0000162
    }

    print("=== СРАВНЕНИЕ ТОЧНОСТИ ФУНКЦИЙ ARCCOS ===")
    print(f"Тестовые данные: {test_cases}\n")

    # Заголовок таблицы
    print(f"{'x':<10} {'Python math.acos':<20} {'Ассемблер':<15} {'Разница':<12} {'Ошибка %':<12}")
    print("-" * 75)

    total_error = 0
    max_error = 0

    for x in test_cases:
        # Вычисление с помощью стандартной библиотеки Python
        python_result = math.acos(x)
        assembler_result = assembler_results[x]
        diff = abs(python_result - assembler_result)

        # Расчет ошибки в процентах
        if python_result != 0:  # Избегаем деления на ноль
            error_percent = (diff / python_result) * 100
        else:
            error_percent = 0

        total_error += error_percent
        max_error = max(max_error, error_percent)

        # Форматируем вывод
        print(f"{x:<10.2f} {python_result:<20.6f} {assembler_result:<15.6f} "
              f"{diff:<12.6f} {error_percent:<12.4f}%")

    # Вывод статистики
    avg_error = total_error / len(test_cases)
    print(f"\nСредняя ошибка: {avg_error:.4f}%")
    print(f"Максимальная ошибка: {max_error:.4f}%")

if __name__ == "__main__":
    main()
