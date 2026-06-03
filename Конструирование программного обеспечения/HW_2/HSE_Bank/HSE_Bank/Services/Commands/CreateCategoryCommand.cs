using HSE_Bank.Models;
using HSE_Bank.Services.Facades;

namespace HSE_Bank.Services.Commands
{
    public class CreateCategoryCommand : ICommand
    {
        private readonly ICategoryFacade _facade;
        private readonly OperationType _type;
        private readonly string _name;

        public CreateCategoryCommand(ICategoryFacade facade, OperationType type, string name)
        {
            _facade = facade;
            _type = type;
            _name = name;
        }

        public void Execute()
        {
            var category = _facade.CreateCategory(_type, _name);
            Console.WriteLine($"Категория создана: {category.Name} (ID: {category.Id}), Тип: {category.Type}");
        }
    }
}