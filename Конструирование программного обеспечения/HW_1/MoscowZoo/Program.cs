using System;
using Microsoft.Extensions.DependencyInjection;
using MoscowZoo.Interfaces;
using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;
using MoscowZoo.Services;
using MoscowZoo.Repository;

namespace MoscowZoo
{
    static class Program
    {
        static void Main(string[] args)
        {
            // Настройка DI-контейнера
            var serviceProvider = DependencyInjection.ConfigureServices();
            var zooService = serviceProvider.GetService<IZooService>();

            Console.WriteLine("Система учета Московского зоопарка");
            Console.WriteLine("Нажмите любую клавишу для продолжения...");
            Console.ReadKey();
            Console.Clear();

            // Добавление тестовых данных
            InitializeTestData(zooService);

            bool exit = false;
            // Основной цикл программы - работает пока пользователь не выберет выход
            while (!exit)
            {
                Console.Clear();
                ShowMenu();
                var input = Console.ReadLine();

                // Обработка выбора пользователя
                switch (input)
                {
                    case "1":
                        Console.Clear();
                        AddNewAnimal(zooService);
                        break;
                    case "2":
                        Console.Clear();
                        zooService.PrintAllAnimals();
                        break;
                    case "3":
                        Console.Clear();
                        zooService.PrintAllInventory();
                        break;
                    case "4":
                        Console.Clear();
                        zooService.PrintContactAnimals();
                        break;
                    case "5":
                        Console.Clear();
                        zooService.PrintFoodNeeds();
                        break;
                    case "6":
                        Console.Clear();
                        AddNewThing(zooService);
                        break;
                    case "0":
                        exit = true; // Устанавливаем флаг выхода из цикла
                        break;
                    default:
                        Console.WriteLine("Неверный ввод. Попробуйте снова.");
                        Console.WriteLine("Нажмите любую клавишу для продолжения...");
                        Console.ReadKey();
                        break;
                }

                // Пауза после выполнения операции (кроме выхода)
                if (!exit && input != "0")
                {
                    Console.WriteLine("\nНажмите любую клавишу для возврата в меню...");
                    Console.ReadKey();
                }
            }

            Console.Clear();
            Console.WriteLine("До свидания!");
        }

        /// <summary>
        /// Отображение главного меню программы
        /// </summary>
        static void ShowMenu()
        {
            Console.WriteLine("=== СИСТЕМА УЧЕТА МОСКОВСКОГО ЗООПАРКА ===");
            Console.WriteLine("\nМеню:");
            Console.WriteLine("1. Добавить новое животное");
            Console.WriteLine("2. Показать всех животных");
            Console.WriteLine("3. Показать весь инвентарь");
            Console.WriteLine("4. Показать животных для контактного зоопарка");
            Console.WriteLine("5. Показать потребность в еде");
            Console.WriteLine("6. Добавить новую вещь");
            Console.WriteLine("0. Выход");
            Console.Write("\nВыберите опцию: ");
        }

        /// <summary>
        /// Добавление нового животного в систему
        /// </summary>
        /// <param name="zooService">Сервис для работы с зоопарком</param>
        static void AddNewAnimal(IZooService zooService)
        {
            Console.WriteLine("=== ДОБАВЛЕНИЕ НОВОГО ЖИВОТНОГО ===");
            
            // Сначала проверяем выбор типа животного
            string typeInput;
            while (true)
            {
                Console.WriteLine("Выберите тип животного:");
                Console.WriteLine("1. Обезьяна");
                Console.WriteLine("2. Кролик");
                Console.WriteLine("3. Тигр");
                Console.WriteLine("4. Волк");
                Console.Write("Ваш выбор: ");
                
                typeInput = Console.ReadLine();
                
                if (typeInput == "1" || typeInput == "2" || typeInput == "3" || typeInput == "4")
                {
                    break; // Корректный выбор, выходим из цикла
                }
                
                Console.WriteLine("ОШИБКА: Неверный тип животного. Выберите от 1 до 4.");
                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }

            // Затем вводим имя животного с валидацией
            string name;
            while (true)
            {
                Console.Write("Введите имя животного: ");
                name = Console.ReadLine();
                
                if (!string.IsNullOrWhiteSpace(name))
                {
                    break; // Имя корректное, выходим из цикла
                }
                
                Console.WriteLine("ОШИБКА: Имя животного не может быть пустым.");
                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }

            // Ввод количества употребляемой еды с валидацией
            int food = GetFoodAmount();

            // Создание объекта животного в зависимости от выбранного типа
            Animal animal = null;
            switch (typeInput)
            {
                case "1":
                    int monkeyKindness = GetKindnessLevel();
                    animal = new Monkey(name, food, 0, monkeyKindness);
                    break;
                case "2":
                    int rabbitKindness = GetKindnessLevel();
                    animal = new Rabbit(name, food, 0, rabbitKindness);
                    break;
                case "3":
                    animal = new Tiger(name, food, 0);
                    break;
                case "4":
                    animal = new Wolf(name, food, 0);
                    break;
            }

            // Попытка добавить животное через сервис
            if (zooService.AddAnimal(animal))
            {
                Console.WriteLine($"\nЖивотное '{name}' успешно добавлено в зоопарк! Инвентарный номер: {animal.Number}");
            }
            else
            {
                Console.WriteLine($"\nЖивотное '{name}' не прошло медосмотр и не может быть добавлено.");
            }
        }
        
        /// <summary>
        /// Получение уровня доброты животного с валидацией
        /// Доброта важна для определения возможности содержания в контактном зоопарке
        /// </summary>
        /// <returns>Уровень доброты от 1 до 10</returns>
        static int GetKindnessLevel()
        {
            // Бесконечный цикл пока пользователь не введет корректное значение
            while (true)
            {
                Console.Write("Введите уровень доброты (1-10): ");
                string input = Console.ReadLine();

                // Парсинг ввода и проверка диапазона
                if (int.TryParse(input, out int kindness))
                {
                    if (kindness >= 1 && kindness <= 10)
                    {
                        return kindness;
                    }
                    else
                    {
                        Console.WriteLine("ОШИБКА: Уровень доброты должен быть от 1 до 10.");
                    }
                }
                else
                {
                    Console.WriteLine("ОШИБКА: Введите целое число от 1 до 10.");
                }

                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }
        }

        /// <summary>
        /// Получение количества еды с валидацией
        /// </summary>
        /// <returns>Количество еды в кг/день</returns>
        static int GetFoodAmount()
        {
            // Бесконечный цикл пока пользователь не введет корректное значение
            while (true)
            {
                Console.Write("Введите количество еды (кг/день): ");
                string input = Console.ReadLine();

                // Парсинг и проверка, что количество положительное
                if (int.TryParse(input, out int food))
                {
                    if (food > 0)
                    {
                        return food;
                    }
                    else if (food == 0)
                    {
                        Console.WriteLine("ОШИБКА: Количество еды не может быть равно нулю.");
                    }
                    else
                    {
                        Console.WriteLine("ОШИБКА: Количество еды не может быть отрицательным.");
                    }
                }
                else
                {
                    Console.WriteLine("ОШИБКА: Введите целое положительное число.");
                }

                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }
        }

        /// <summary>
        /// Добавление новой вещи в инвентарь
        /// </summary>
        /// <param name="zooService">Сервис для работы с зоопарком</param>
        static void AddNewThing(IZooService zooService)
        {
            Console.WriteLine("=== ДОБАВЛЕНИЕ НОВОЙ ВЕЩИ ===");
    
            // Сначала проверяем выбор типа вещи
            string typeInput;
            while (true)
            {
                Console.WriteLine("Выберите тип вещи:");
                Console.WriteLine("1. Стол");
                Console.WriteLine("2. Компьютер");
                Console.Write("Ваш выбор: ");
        
                typeInput = Console.ReadLine();
        
                if (typeInput == "1" || typeInput == "2")
                {
                    break; // Корректный выбор, выходим из цикла
                }
        
                Console.WriteLine("ОШИБКА: Неверный тип вещи. Выберите 1 или 2.");
                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }

            // Затем вводим название вещи с валидацией
            string name;
            while (true)
            {
                Console.Write("Введите название вещи: ");
                name = Console.ReadLine();
        
                if (!string.IsNullOrWhiteSpace(name))
                {
                    break; // Название корректное, выходим из цикла
                }
        
                Console.WriteLine("ОШИБКА: Название вещи не может быть пустым.");
                Console.WriteLine("Пожалуйста, попробуйте снова.\n");
            }

            Thing thing = null;
            switch (typeInput)
            {
                case "1":
                    thing = new Table(name, 0);
                    break;
                case "2":
                    thing = new Computer(name, 0);
                    break;
            }

            zooService.AddThing(thing);
            Console.WriteLine($"\nВещь '{name}' успешно добавлена в инвентарь! Инвентарный номер: {thing.Number}");
        }

        /// <summary>
        /// Инициализация тестовых данных для демонстрации работы системы
        /// </summary>
        /// <param name="zooService">Сервис для работы с зоопарком</param>
        static void InitializeTestData(IZooService zooService)
        {
            Console.WriteLine("=== ЗАГРУЗКА ТЕСТОВЫХ ДАННЫХ ===");

            // Создание массива тестовых животных
            // Номер 0 передается временно - реальный номер устанавливается в репозитории
            var testAnimals = new Animal[]
            {
                new Monkey("Чита", 3, 0, 8),        // Подходит для контактного зоопарка (доброта >= 5)
                new Rabbit("Пушистик", 1, 0, 10),   // Подходит для контактного зоопарка (доброта >= 5)
                new Tiger("Шерхан", 10, 0),                     // Не подходит (хищник)
                new Rabbit("Морковка", 2, 0, 3),    // Не подходит (доброта < 5)
                new Wolf("Серый", 6, 0)                         // Не подходит (хищник)
            };

            // Добавление всех тестовых животных в систему
            foreach (var animal in testAnimals)
            {
                // Установить здоровье для тестовых данных
                // В реальной системе здоровье проверялось бы при добавлении
                animal.IsHealthy = true;
                zooService.AddAnimalWithoutInteractive(animal);
            }

            // Добавление тестовых вещей
            zooService.AddThing(new Table("Рабочий стол", 0));
            zooService.AddThing(new Computer("Компьютер администратора", 0));

            Console.WriteLine("Тестовые данные загружены.");
            Console.WriteLine("Нажмите любую клавишу для продолжения...");
            Console.ReadKey();
            Console.Clear();
        }
    }
}