using MoscowZoo.Interfaces;
using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;

namespace MoscowZoo.Repository
{
    /// <summary>
    /// Реализация репозитория для управления данными зоопарка
    /// </summary>
    public class ZooRepository : IZooRepository
    {
        // Внутренние коллекции для хранения данных зоопарка
        private readonly List<Animal> _animals = new List<Animal>();
        private readonly List<Thing> _things = new List<Thing>();

        // Счетчики для автоматической нумерации инвентарных номеров
        private int _nextAnimalNumber = 1;  // Начинаем нумерацию животных с 1
        private int _nextThingNumber = 1;   // Начинаем нумерацию вещей с 1

        /// <summary>
        /// Добавляет животное в репозиторий с автоматической нумерацией
        /// Присваивает уникальный инвентарный номер каждому животному
        /// </summary>
        public void AddAnimal(Animal animal)
        {
            // Присваиваем следующий доступный номер и увеличиваем счетчик
            animal.Number = _nextAnimalNumber++;
            _animals.Add(animal);
        }

        /// <summary>
        /// Добавляет вещь в репозиторий с автоматической нумерацией
        /// Присваивает уникальный инвентарный номер каждой вещи
        /// </summary>
        public void AddThing(Thing thing)
        {
            // Присваиваем следующий доступный номер и увеличиваем счетчик
            thing.Number = _nextThingNumber++;
            _things.Add(thing);
        }

        /// <summary>
        /// Возвращает всех животных зоопарка
        /// </summary>
        public IEnumerable<Animal> GetAllAnimals() => _animals;

        /// <summary>
        /// Возвращает все вещи из инвентаря зоопарка
        /// </summary>
        public IEnumerable<Thing> GetAllThings() => _things;

        /// <summary>
        /// Возвращает животных, подходящих для контактного зоопарка
        /// Фильтрует по интерфейсу IHerbivore и свойству CanBeInContact
        /// </summary>
        public IEnumerable<Animal> GetAnimalsForContact() =>
            _animals.OfType<IHerbivorous>()                       // Выбираем только травоядных животных
                    .Where(h => h.CanBeInContactZoo)   // Фильтруем по возможности контакта
                    .Cast<Animal>();                              // Преобразуем обратно к типу Animal

        /// <summary>
        /// Рассчитывает общую потребность в еде для всех животных
        /// Суммирует свойство Food каждого животного
        /// </summary>
        public int GetTotalFoodNeeds() => _animals.Sum(a => a.Food);
    }
}