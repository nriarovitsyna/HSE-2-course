namespace MoscowZoo.Models.Animals
{
    /// <summary>
    /// Обезьяна - травоядное животное
    /// </summary>
    public class Monkey : Herbivorous
    {
        public Monkey(string name, int food, int number, int kindnessLevel)
            : base(name, food, number, kindnessLevel)
        {
        }
    }
}