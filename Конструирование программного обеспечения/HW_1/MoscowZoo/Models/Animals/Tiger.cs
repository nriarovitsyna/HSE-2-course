namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Тигр - хищник
    /// </summary>
    public class Tiger : Predator
    {
        public Tiger(string name, int food, int number)
            : base(name, food, number)
        {
        }
    }
}