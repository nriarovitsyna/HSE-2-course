using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public class CategoryFactory : ICategoryFactory
    {
        private static int _nextId = 1;

        public Category CreateCategory(OperationType type, string name)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Название категории не может быть пустым");

            return new Category(_nextId++, type, name);
        }
    }
}