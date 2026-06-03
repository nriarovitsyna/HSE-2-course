using HSE_Bank.Models;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Commands;

namespace HSE_Bank.Utilities
{
    public class CategoryManager
    {
        private readonly ICategoryFacade _facade;

        public CategoryManager(ICategoryFacade facade)
        {
            _facade = facade;
        }

        public void ManageCategories()
        {
            while (true)
            {
                ConsoleHelper.PrintHeader("Управление категориями");

                var categories = _facade.GetAllCategories();
                ConsoleHelper.PrintCategories(categories);

                Console.WriteLine("\nДоступные действия:");
                Console.WriteLine("1. Создать категорию доходов");
                Console.WriteLine("2. Создать категорию расходов");
                Console.WriteLine("3. Редактировать категорию");
                Console.WriteLine("4. Удалить категорию");
                Console.WriteLine("5. Назад в главное меню");
                Console.WriteLine(new string('-', 40));

                var choice = ConsoleHelper.ReadInt("Выберите действие");

                switch (choice)
                {
                    case 1: CreateCategory(OperationType.Income); break;
                    case 2: CreateCategory(OperationType.Expense); break;
                    case 3: EditCategory(categories); break;
                    case 4: DeleteCategory(categories); break;
                    case 5: return;
                    default: ConsoleHelper.PrintError("Неверный выбор"); break;
                }

                ConsoleHelper.WaitForContinue();
            }
        }

        private void CreateCategory(OperationType type)
        {
            var typeName = type == OperationType.Income ? "доходов" : "расходов";
            ConsoleHelper.PrintHeader($"Создание категории {typeName}");

            var name = ConsoleHelper.ReadString("Название категории");

            var command = new CreateCategoryCommand(_facade, type, name);
            var decoratedCommand = new CommandDecorator(command);
            decoratedCommand.Execute();
        }

        private void EditCategory(List<Category> categories)
        {
            if (!categories.Any()) return;

            var categoryId = ConsoleHelper.ReadInt("Введите ID категории для редактирования");
            var category = _facade.GetCategory(categoryId);

            if (category == null)
            {
                ConsoleHelper.PrintError("Категория не найдена");
                return;
            }

            Console.WriteLine($"\nРедактирование категории: {category.Name}");
            var newName = ConsoleHelper.ReadString($"Новое название (текущее: {category.Name})");

            if (!string.IsNullOrWhiteSpace(newName))
            {
                category.Name = newName;
                _facade.UpdateCategory(category);
                ConsoleHelper.PrintSuccess("Категория успешно обновлена!");
            }
        }

        private void DeleteCategory(List<Category> categories)
        {
            if (!categories.Any()) return;

            var categoryId = ConsoleHelper.ReadInt("Введите ID категории для удаления");
            var category = _facade.GetCategory(categoryId);

            if (category == null)
            {
                ConsoleHelper.PrintError("Категория не найдена");
                return;
            }

            if (ConsoleHelper.ConfirmAction($"Вы уверены, что хотите удалить категорию '{category.Name}'?"))
            {
                _facade.DeleteCategory(categoryId);
                ConsoleHelper.PrintSuccess("Категория успешно удалена!");
            }
        }
    }
}