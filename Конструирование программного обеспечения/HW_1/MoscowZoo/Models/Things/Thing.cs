using MoscowZoo.Interfaces;

namespace MoscowZoo.Models.Things
{
    /// <summary>
    /// Абстрактный базовый класс для всех вещей в инвентаре зоопарка
    /// Реализует интерфейс IInventory (объект инвентаря)
    /// </summary>
    public abstract class Thing : IInventory
    {
        public string Name { get; protected set; }  // Название вещи
        public int Number { get; set; }             // Инвентарный номер (устанавливается репозиторием)

        /// <summary>
        /// Конструктор базового класса Thing
        /// </summary>
        /// <param name="name">Название вещи</param>
        /// <param name="number">Инвентарный номер (обычно 0, устанавливается репозиторием)</param>
        protected Thing(string name, int number)
        {
            Name = name;
            Number = number;
        }

        /// <summary>
        /// Возвращает информацию об объекте для инвентаризации
        /// Виртуальный метод - может быть переопределен в наследниках
        /// </summary>
        public virtual string GetInventoryInfo() => $"{Name} (№{Number})";
    }
}