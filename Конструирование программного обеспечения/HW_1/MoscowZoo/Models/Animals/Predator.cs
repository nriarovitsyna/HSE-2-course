namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Абстрактный класс для хищных животных
    /// Наследуется от базового класса Animal и реализует общие характеристики хищников
    /// </summary>
    public abstract class Predator : Animal
    {
        /// <summary>
        /// Конструктор для создания хищного животного
        /// </summary>
        /// <param name="name">Имя животного</param>
        /// <param name="food">Количество еды в кг/день</param>
        /// <param name="number">Инвентарный номер (передается 0, т.к. номер присваивается автоматически в репозитории)</param>
        protected Predator(string name, int food, int number)
            : base(name, food, number)
        {
        }
    }
}