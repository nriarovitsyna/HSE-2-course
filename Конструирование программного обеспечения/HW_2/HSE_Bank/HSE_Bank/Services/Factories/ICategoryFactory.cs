using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public interface ICategoryFactory
    {
        Category CreateCategory(OperationType type, string name);
    }
}