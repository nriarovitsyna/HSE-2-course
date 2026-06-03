using MoscowZoo.Interfaces;

namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Абстрактный базовый класс для травоядных животных
    /// Наследует от Animal и реализует интерфейс IHerbivore
    /// </summary>
    public abstract class Herbivorous : Animal, IHerbivorous
    {
        public int KindnessLevel { get; protected set; }  // Уровень доброты животного (1-10)

        // Вычисляемое свойство - можно ли содержать в контактном зоопарке
        // Доброта больше 5 означает, что животное безопасно для контакта
        public bool CanBeInContactZoo => KindnessLevel > 5;

        /// <summary>
        /// Конструктор для травоядных животных
        /// </summary>
        /// <param name="name">Имя животного</param>
        /// <param name="food">Количество еды в кг/день</param>
        /// <param name="number">Инвентарный номер</param>
        /// <param name="kindnessLevel">Уровень доброты (1-10)</param>
        protected Herbivorous(string name, int food, int number, int kindnessLevel)
            : base(name, food, number)
        {
            KindnessLevel = kindnessLevel;
        }

        /// <summary>
        /// Возвращает строковое представление травоядного животного
        /// Добавляет информацию о доброте и возможности контакта к базовому выводу
        /// </summary>
        public override string ToString() =>
            $"{base.ToString()}, Доброта: {KindnessLevel}/10, Контактный зоопарк: {(CanBeInContactZoo ? "Да" : "Нет")}";
    }
}