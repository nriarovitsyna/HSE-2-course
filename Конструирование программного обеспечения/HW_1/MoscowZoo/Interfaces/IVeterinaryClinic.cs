using MoscowZoo.Models.Animals;

namespace MoscowZoo.Interfaces
{
    /// <summary>
    /// Интерфейс ветеринарной клиники для проверки здоровья животных
    /// </summary>
    public interface IVeterinaryClinic
    {
        bool CheckHealth(Animal animal);
        bool CheckHealth(Animal animal, bool isHealthy);
    }
}