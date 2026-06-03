using Xunit;
using MoscowZoo.Interfaces;
using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;
using MoscowZoo.Services;
using MoscowZoo.Repository;

namespace Test
{
    public class ZooTests
    {
        private readonly IZooService _zooService;
        private readonly IZooRepository _repository;
        private readonly IVeterinaryClinic _clinic;

        // Конструктор тестового класса - выполняется перед каждым тестом
        public ZooTests()
        {
            // Ручное создание зависимостей без DI-контейнера для unit-тестов
            _clinic = new VeterinaryClinic();
            _repository = new ZooRepository();
            // ZooService зависит от репозитория и ветеринарной клиники (dependency injection)
            _zooService = new ZooService(_repository, _clinic);
        }

        // Тесты для ZooService

        /// <summary>
        /// Тест проверяет добавление здорового животного через сервис
        /// Ожидается, что метод вернет true
        /// </summary>
        [Fact]
        public void AddAnimal_HealthyAnimal_ReturnsTrue()
        {
            // Arrange (подготовка данных)
            var rabbit = new Rabbit("Крош", 1, 0, 8);

            // Act (выполнение тестируемого действия)
            bool result = _zooService.AddAnimalWithoutInteractive(rabbit);

            // Assert (проверка результата)
            Assert.True(result);
        }

        /// <summary>
        /// Тест проверяет добавление вещи через сервис
        /// Ожидается, что вещь появится в репозитории
        /// </summary>
        [Fact]
        public void AddThing_ShouldAddThingToRepository()
        {
            // Arrange
            var table = new Table("Письменный стол", 0);

            // Act
            _zooService.AddThing(table);

            // Assert
            var things = _repository.GetAllThings();
            Assert.Contains(table, things);
        }

        /// <summary>
        /// Тест проверяет, что методы вывода не выбрасывают исключения
        /// </summary>
        [Fact]
        public void PrintMethods_ShouldNotThrowExceptions()
        {
            // Arrange
            var rabbit = new Rabbit("Банни", 1, 0, 8);
            var table = new Table("Кухонный стол", 0);
            _zooService.AddAnimalWithoutInteractive(rabbit);
            _zooService.AddThing(table);

            // Act & Assert
            Assert.Null(Record.Exception(() => _zooService.PrintAllAnimals()));
            Assert.Null(Record.Exception(() => _zooService.PrintAllInventory()));
            Assert.Null(Record.Exception(() => _zooService.PrintContactAnimals()));
            Assert.Null(Record.Exception(() => _zooService.PrintFoodNeeds()));
        }

        // Тесты для ZooRepository

        /// <summary>
        /// Тест проверяет корректность фильтрации животных для контактного зоопарка
        /// </summary>
        [Fact]
        public void GetAnimalsForContact_ShouldReturnKindAnimals()
        {
            // Arrange
            var kindRabbit = new Rabbit("Добрый кролик", 1, 0, 8);
            var angryRabbit = new Rabbit("Злой кролик", 1, 0, 2);
            var tiger = new Tiger("Шархан", 10, 0);

            _repository.AddAnimal(kindRabbit);
            _repository.AddAnimal(angryRabbit);
            _repository.AddAnimal(tiger);

            // Act
            var contactAnimals = _repository.GetAnimalsForContact();

            // Assert
            Assert.Single(contactAnimals);
            Assert.Contains(kindRabbit, contactAnimals);
            Assert.DoesNotContain(angryRabbit, contactAnimals);
            Assert.DoesNotContain(tiger, contactAnimals);
        }

        /// <summary>
        /// Тест проверяет корректность расчета общего количества необходимой еды
        /// </summary>
        [Fact]
        public void GetTotalFoodRequired_ShouldReturnCorrectSum()
        {
            // Arrange
            var rabbit = new Rabbit("Джуди", 1, 0, 8);
            var tiger = new Tiger("Диего", 10, 0);
            var wolf = new Wolf("Акела", 6, 0);

            _repository.AddAnimal(rabbit);
            _repository.AddAnimal(tiger);
            _repository.AddAnimal(wolf);

            // Act
            int totalFood = _repository.GetTotalFoodNeeds();

            // Assert
            Assert.Equal(17, totalFood);
        }

        /// <summary>
        /// Тест проверяет, что репозиторий возвращает всех добавленных животных
        /// </summary>
        [Fact]
        public void GetAllAnimals_ShouldReturnAllAddedAnimals()
        {
            // Arrange
            var rabbit = new Rabbit("Лола Банни", 1, 0, 8);
            var monkey = new Monkey("Абу", 3, 0, 6);

            _repository.AddAnimal(rabbit);
            _repository.AddAnimal(monkey);

            // Act
            var animals = _repository.GetAllAnimals();

            // Assert
            Assert.Equal(2, animals.Count());
            Assert.Contains(rabbit, animals);
            Assert.Contains(monkey, animals);
        }

        /// <summary>
        /// Тест проверяет, что репозиторий возвращает все добавленные вещи
        /// </summary>
        [Fact]
        public void GetAllThings_ShouldReturnAllAddedThings()
        {
            // Arrange
            var table = new Table("ИКЕА стол", 0);
            var computer = new Computer("MacBook", 0);

            _repository.AddThing(table);
            _repository.AddThing(computer);

            // Act
            var things = _repository.GetAllThings();

            // Assert
            Assert.Equal(2, things.Count());
            Assert.Contains(table, things);
            Assert.Contains(computer, things);
        }

        // Тесты для VeterinaryClinic

        /// <summary>
        /// Тест проверяет, что ветеринарная клиника корректно устанавливает здоровье животного
        /// </summary>
        [Fact]
        public void CheckHealth_ShouldSetHealthAnimals()
        {
            // Arrange
            var animal = new Rabbit("Здоровый кролик", 1, 0, 8);

            // Act
            bool result = _clinic.CheckHealth(animal, true);

            // Assert
            Assert.Equal(result, animal.IsHealthy);
        }

        // Тесты для животных и интерфейсов

        /// <summary>
        /// Тест проверяет, что животное корректно реализует необходимые интерфейсы
        /// </summary>
        [Fact]
        public void Animal_ImplementsInterfaces_Correctly()
        {
            // Arrange
            var rabbit = new Rabbit("Морковка", 1, 0, 8);

            // Act & Assert
            Assert.IsAssignableFrom<IAlive>(rabbit);
            Assert.IsAssignableFrom<IInventory>(rabbit);
            Assert.IsAssignableFrom<IHerbivorous>(rabbit);
        }

        /// <summary>
        /// Тест проверяет реализацию интерфейсов обезьяной
        /// </summary>
        [Fact]
        public void Monkey_ShouldImplementCorrectInterfaces()
        {
            // Arrange
            var monkey = new Monkey("Бамбино", 3, 0, 6);

            // Act & Assert
            Assert.IsAssignableFrom<IAlive>(monkey);
            Assert.IsAssignableFrom<IInventory>(monkey);
            Assert.IsAssignableFrom<IHerbivorous>(monkey);
        }

        /// <summary>
        /// Тест проверяет реализацию интерфейсов тигром
        /// </summary>
        [Fact]
        public void Tiger_ShouldImplementCorrectInterfaces()
        {
            // Arrange
            var tiger = new Tiger("Виталий", 10, 0);

            // Act & Assert
            Assert.IsAssignableFrom<IAlive>(tiger);
            Assert.IsAssignableFrom<IInventory>(tiger);
            Assert.False(tiger is IHerbivorous);
        }

        /// <summary>
        /// Тест проверяет реализацию интерфейсов волком
        /// </summary>
        [Fact]
        public void Wolf_ShouldImplementCorrectInterfaces()
        {
            // Arrange
            var wolf = new Wolf("Серый Волк", 6, 0);

            // Act & Assert
            Assert.IsAssignableFrom<IAlive>(wolf);
            Assert.IsAssignableFrom<IInventory>(wolf);
            Assert.False(wolf is IHerbivorous);
        }

        /// <summary>
        /// Тест проверяет логику определения возможности нахождения в контактном зоопарке
        /// </summary>
        [Fact]
        public void Herbivore_CanBeInContactZoo_WhenKindnessAbove()
        {
            // Arrange
            var kindRabbit = new Rabbit("Добрый Банни", 1, 0, 8);
            var angryRabbit = new Rabbit("Злой Банни", 1, 0, 3);

            // Act & Assert
            Assert.True(kindRabbit.CanBeInContactZoo);
            Assert.False(angryRabbit.CanBeInContactZoo);
        }

        // Тесты для вещей

        /// <summary>
        /// Тест проверяет, что вещи реализуют интерфейс инвентаря
        /// </summary>
        [Fact]
        public void Thing_ImplementsInventoryInterface()
        {
            // Arrange
            var table = new Table("Хофф Стол", 0);
            var computer = new Computer("Lenovo", 0);

            // Act & Assert
            Assert.IsAssignableFrom<IInventory>(table);
            Assert.IsAssignableFrom<IInventory>(computer);
        }

        /// <summary>
        /// Тест проверяет корректность получения информации об инвентаре для вещи
        /// </summary>
        [Fact]
        public void Thing_GetInventoryInfo_ReturnsCorrectInfo()
        {
            // Arrange
            var table = new Table("Рабочий стол", 0);

            // Act
            var info = table.GetInventoryInfo();

            // Assert
            Assert.Equal("Рабочий стол (№0)", info);
        }

        /// <summary>
        /// Тест проверяет корректность получения информации об инвентаре для животного
        /// </summary>
        [Fact]
        public void Animal_GetInventoryInfo_ReturnsCorrectInfo()
        {
            // Arrange
            var rabbit = new Rabbit("Заяц", 1, 0, 8);

            // Act
            var info = rabbit.GetInventoryInfo();

            // Assert
            Assert.Equal("Заяц (№0)", info);
        }

        // Новые тесты для проверки автоматической нумерации

        /// <summary>
        /// Тест проверяет автоматическую нумерацию животных в репозитории
        /// </summary>
        [Fact]
        public void Animals_ShouldGetAutoIncrementedNumbers()
        {
            // Arrange
            var rabbit = new Rabbit("Флэш", 1, 0, 8);
            var tiger = new Tiger("Полосатый", 10, 0);
            var wolf = new Wolf("Альберто", 6, 0);

            // Act
            _repository.AddAnimal(rabbit);
            _repository.AddAnimal(tiger);
            _repository.AddAnimal(wolf);

            // Assert
            Assert.Equal(1, rabbit.Number);
            Assert.Equal(2, tiger.Number);
            Assert.Equal(3, wolf.Number);
        }

        /// <summary>
        /// Тест проверяет автоматическую нумерацию вещей в репозитории
        /// </summary>
        [Fact]
        public void Things_ShouldGetAutoIncrementedNumbers()
        {
            // Arrange
            var table = new Table("Столик", 0);
            var computer = new Computer("Huawei", 0);

            // Act
            _repository.AddThing(table);
            _repository.AddThing(computer);

            // Assert
            Assert.Equal(1, table.Number);
            Assert.Equal(2, computer.Number);
        }
    }
}