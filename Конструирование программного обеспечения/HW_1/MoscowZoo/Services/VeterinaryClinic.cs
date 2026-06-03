using MoscowZoo.Interfaces;
using MoscowZoo.Models.Animals;

namespace MoscowZoo.Services
{
    /// <summary>
    /// Ветеринарная клиника для проведения медосмотров животных
    /// </summary>
    public class VeterinaryClinic : IVeterinaryClinic
    {
        /// <summary>
        /// Проводит медосмотр животного через интерактивный диалог с пользователем
        /// </summary>
        public bool CheckHealth(Animal animal)
        {
            Console.WriteLine($"\n=== МЕДОСМОТР ЖИВОТНОГО ===");
            Console.WriteLine($"Животное: {animal.Name}");
            Console.WriteLine($"Тип: {animal.GetType().Name}");

            // Бесконечный цикл пока не получим корректный ответ
            while (true)
            {
                Console.Write("Животное здорово? (y/n/да/нет): ");
                var input = Console.ReadLine()?.ToLower().Trim();

                // Проверка пустого ввода
                if (string.IsNullOrWhiteSpace(input))
                {
                    Console.WriteLine("ОШИБКА: Введите ответ.");
                    continue;
                }

                // Проверка положительного ответа (поддерживает русский и английский)
                if (input == "y" || input == "yes" || input == "да" || input == "д")
                {
                    animal.IsHealthy = true;  // Устанавливаем флаг здоровья
                    return true;              // Возвращаем успешный результат
                }
                // Проверка отрицательного ответа (поддерживает русский и английский)
                else if (input == "n" || input == "no" || input == "нет" || input == "н")
                {
                    animal.IsHealthy = false; // Устанавливаем флаг болезни
                    return false;             // Возвращаем отрицательный результат
                }
                else
                {
                    // Некорректный ввод - показываем подсказку
                    Console.WriteLine("ОШИБКА: Неверный ввод. Используйте: y/n/да/нет");
                    Console.WriteLine("Подсказка:");
                    Console.WriteLine("  y/yes/да/д - животное здорово");
                    Console.WriteLine("  n/no/нет/н - животное не здорово");
                }
            }
        }
        /// <summary>
        /// Проводит медосмотр животного для тестирования
        /// </summary>
        public bool CheckHealth(Animal animal, bool isHealthy)
        {
            animal.IsHealthy = isHealthy;
            return isHealthy;
        }
    }
}