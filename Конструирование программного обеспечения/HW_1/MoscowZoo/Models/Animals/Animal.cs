using MoscowZoo.Interfaces;

namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Абстрактный базовый класс для всех животных в зоопарке
    /// Реализует интерфейсы IAlive (живое существо) и IInventory (объект инвентаря)
    /// </summary>
    public abstract class Animal : IAlive, IInventory
    {
        public string Name { get; protected set; }  // Имя животного (можно установить только в наследниках)
        public int Food { get; protected set; }     // Количество еды в кг/день
        public int Number { get; set; }             // Инвентарный номер (устанавливается репозиторием)
        public bool IsHealthy { get; set; }         // Состояние здоровья животного

        /// <summary>
        /// Конструктор базового класса Animal
        /// </summary>
        /// <param name="name">Имя животного</param>
        /// <param name="food">Количество еды в кг/день</param>
        /// <param name="number">Инвентарный номер (обычно 0, устанавливается репозиторием)</param>
        protected Animal(string name, int food, int number)
        {
            Name = name;
            Food = food;
            Number = number;
            IsHealthy = true;  // По умолчанию считаем животное здоровым
        }

        /// <summary>
        /// Возвращает информацию об объекте для инвентаризации
        /// Виртуальный метод - может быть переопределен в наследниках
        /// </summary>
        public virtual string GetInventoryInfo() => $"{Name} (№{Number})";

        /// <summary>
        /// Возвращает строковое представление животного
        /// Переопределяет стандартный метод ToString()
        /// </summary>
        public override string ToString() => $"{GetType().Name}: {Name}, Еда: {Food}кг/день";
    }
}