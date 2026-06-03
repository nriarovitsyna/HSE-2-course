using MoscowZoo.Models.Animals;
using MoscowZoo.Models.Things;

namespace MoscowZoo.Interfaces
{
    /// <summary>
    /// Интерфейс для управления операциями зоопарка
    /// </summary>
    public interface IZooService
    {
        bool AddAnimal(Animal animal); 
        bool AddAnimalWithoutInteractive(Animal animal, bool isHealthy = true);
        void AddThing(Thing thing);
        void PrintAllAnimals();
        void PrintAllInventory();
        void PrintContactAnimals();
        void PrintFoodNeeds();
    }
}