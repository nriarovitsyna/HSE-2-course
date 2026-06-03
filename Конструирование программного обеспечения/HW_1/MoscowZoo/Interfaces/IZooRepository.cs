using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;

namespace MoscowZoo.Interfaces
{
    /// <summary>
    /// Интерфейс репозитория для управления данными зоопарка
    /// </summary>
    public interface IZooRepository
    {
        void AddAnimal(Animal animal);
        void AddThing(Thing thing);
        IEnumerable<Animal> GetAllAnimals();
        IEnumerable<Thing> GetAllThings();
        IEnumerable<Animal> GetAnimalsForContact();
        int GetTotalFoodNeeds();
    }
}