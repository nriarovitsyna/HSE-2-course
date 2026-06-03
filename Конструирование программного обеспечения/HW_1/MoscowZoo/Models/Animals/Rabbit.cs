namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Кролик - травоядное животное
    /// </summary>
    public class Rabbit : Herbivorous
    {
        public Rabbit(string name, int food, int number, int kindnessLevel)
            : base(name, food, number, kindnessLevel)
        {
        }
    }
}