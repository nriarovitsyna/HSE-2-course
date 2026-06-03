using MoscowZoo.Interfaces;
using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;

namespace MoscowZoo.Services
{
    /// <summary>
    /// Основной сервис для управления операциями зоопарка
    /// </summary>
    public class ZooService : IZooService
    {
        private readonly IZooRepository _repository;
        private readonly IVeterinaryClinic _clinic;

        /// <summary>
        /// Конструктор с внедрением зависимостей через интерфейсы
        /// Позволяет легко подменять реализации репозитория и клиники
        /// </summary>
        public ZooService(IZooRepository repository, IVeterinaryClinic clinic)
        {
            _repository = repository;
            _clinic = clinic;
        }

        /// <summary>
        /// Добавляет животное в зоопарк после прохождения медосмотра
        /// Возвращает true если животное здорово и добавлено успешно
        /// </summary>
        public bool AddAnimal(Animal animal)
        {
            // Сначала проверяем здоровье животного через ветеринарную клинику
            if (_clinic.CheckHealth(animal))
            {
                // Если животное здорово - добавляем в репозиторий
                _repository.AddAnimal(animal);
                return true;
            }
            return false;
        }
        
        /// <summary>
        /// Добавляет животное без интерактивного медосмотра (для тестовых данных)
        /// </summary>
        public bool AddAnimalWithoutInteractive(Animal animal, bool isHealthy = true)
        {
            // Сначала проверяем здоровье животного через ветеринарную клинику
            if (_clinic.CheckHealth(animal, isHealthy))
            {
                // Если животное здорово - добавляем в репозиторий
                _repository.AddAnimal(animal);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Добавляет вещь в инвентарь зоопарка
        /// Не требует дополнительных проверок
        /// </summary>
        public void AddThing(Thing thing) => _repository.AddThing(thing);

        /// <summary>
        /// Выводит в консоль всех животных зоопарка
        /// Использует переопределенный метод ToString() каждого животного
        /// </summary>
        public void PrintAllAnimals()
        {
            var animals = _repository.GetAllAnimals();
            Console.WriteLine("\nВСЕ ЖИВОТНЫЕ ЗООПАРКА");
            foreach (var animal in animals)
            {
                Console.WriteLine(animal);
            }
        }

        /// <summary>
        /// Выводит в консоль весь инвентарь зоопарка
        /// Использует метод GetInventoryInfo() для получения информации о вещах
        /// </summary>
        public void PrintAllInventory()
        {
            var things = _repository.GetAllThings();

            Console.WriteLine("\nВЕСЬ ИНВЕНТАРЬ ЗООПАРКА");
            foreach (var thing in things)
            {
                Console.WriteLine($"  - {thing.GetInventoryInfo()}");
            }
        }

        /// <summary>
        /// Выводит в консоль животных, подходящих для контактного зоопарка
        /// Фильтрация происходит на уровне репозитория по правилам
        /// </summary>
        public void PrintContactAnimals()
        {
            var contactAnimals = _repository.GetAnimalsForContact();
            Console.WriteLine("\nЖИВОТНЫЕ ДЛЯ КОНТАКТНОГО ЗООПАРКА");
            foreach (var animal in contactAnimals)
            {
                Console.WriteLine(animal);
            }
        }

        /// <summary>
        /// Выводит общую потребность зоопарка в еде
        /// Суммирует потребности в еде всех животных
        /// </summary>
        public void PrintFoodNeeds()
        {
            int totalFood = _repository.GetTotalFoodNeeds();
            Console.WriteLine($"\nПОТРЕБНОСТЬ В ЕДЕ");
            Console.WriteLine($"Общее количество еды в день: {totalFood}кг");
        }
    }
}