namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Волк - хищник
    /// </summary>
    public class Wolf : Predator
    {
        public Wolf(string name, int food, int number)
            : base(name, food, number)
        {
        }
    }
}