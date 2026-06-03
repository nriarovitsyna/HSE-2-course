using HSE_Bank.Models;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Commands;

namespace HSE_Bank.Utilities
{
    public class OperationManager
    {
        private readonly IOperationFacade _operationFacade;
        private readonly IBankAccountFacade _accountFacade;
        private readonly ICategoryFacade _categoryFacade;

        public OperationManager(IOperationFacade operationFacade, IBankAccountFacade accountFacade, ICategoryFacade categoryFacade)
        {
            _operationFacade = operationFacade;
            _accountFacade = accountFacade;
            _categoryFacade = categoryFacade;
        }

        public void ManageOperations()
        {
            while (true)
            {
                ConsoleHelper.PrintHeader("Управление операциями");

                var operations = _operationFacade.GetAllOperations();
                var accounts = _accountFacade.GetAllAccounts();
                var categories = _categoryFacade.GetAllCategories();

                ConsoleHelper.PrintOperations(operations, categories, accounts);

                Console.WriteLine("\nДоступные действия:");
                Console.WriteLine("1. Создать операцию дохода");
                Console.WriteLine("2. Создать операцию расхода");
                Console.WriteLine("3. Редактировать операцию");
                Console.WriteLine("4. Удалить операцию");
                Console.WriteLine("5. Назад в главное меню");
                Console.WriteLine(new string('-', 40));

                var choice = ConsoleHelper.ReadInt("Выберите действие");

                switch (choice)
                {
                    case 1: CreateOperation(OperationType.Income); break;
                    case 2: CreateOperation(OperationType.Expense); break;
                    case 3: EditOperation(operations); break;
                    case 4: DeleteOperation(operations); break;
                    case 5: return;
                    default: ConsoleHelper.PrintError("Неверный выбор"); break;
                }

                ConsoleHelper.WaitForContinue();
            }
        }

        private void CreateOperation(OperationType type)
        {
            var typeName = type == OperationType.Income ? "дохода" : "расхода";
            ConsoleHelper.PrintHeader($"Создание операции {typeName}");

            var accounts = _accountFacade.GetAllAccounts();
            if (!accounts.Any())
            {
                ConsoleHelper.PrintError("Нет доступных счетов. Сначала создайте счет.");
                return;
            }

            var categories = _categoryFacade.GetCategoriesByType(type);
            if (!categories.Any())
            {
                ConsoleHelper.PrintError($"Нет доступных категорий {typeName}. Сначала создайте категорию.");
                return;
            }

            ConsoleHelper.PrintAccounts(accounts);
            var accountId = ConsoleHelper.ReadInt("ID счета");

            ConsoleHelper.PrintCategories(categories);
            var categoryId = ConsoleHelper.ReadInt("ID категории");

            var amount = ConsoleHelper.ReadDecimal("Сумма операции");
            var date = ConsoleHelper.ReadDate("Дата операции");
            var description = ConsoleHelper.ReadString("Описание операции");

            var command = new CreateOperationCommand(_operationFacade, type, accountId, amount, date, description, categoryId);
            var decoratedCommand = new CommandDecorator(command);
            decoratedCommand.Execute();
        }

        private void EditOperation(List<Operation> operations)
        {
            if (!operations.Any()) return;

            var operationId = ConsoleHelper.ReadInt("Введите ID операции для редактирования");
            var operation = _operationFacade.GetOperation(operationId);

            if (operation == null)
            {
                ConsoleHelper.PrintError("Операция не найдена");
                return;
            }

            Console.WriteLine($"\nРедактирование операции ID: {operation.Id}");

            var newAmount = ConsoleHelper.ReadDecimal($"Новая сумма (текущая: {operation.Amount:C})");
            var newDate = ConsoleHelper.ReadDate($"Новая дата (текущая: {operation.Date:dd.MM.yyyy})");
            var newDescription = ConsoleHelper.ReadString($"Новое описание (текущее: {operation.Description})");

            operation.Amount = newAmount;
            operation.Date = newDate;
            operation.Description = newDescription;

            _operationFacade.UpdateOperation(operation);
            ConsoleHelper.PrintSuccess("Операция успешно обновлена!");
        }

        private void DeleteOperation(List<Operation> operations)
        {
            if (!operations.Any()) return;

            var operationId = ConsoleHelper.ReadInt("Введите ID операции для удаления");
            var operation = _operationFacade.GetOperation(operationId);

            if (operation == null)
            {
                ConsoleHelper.PrintError("Операция не найдена");
                return;
            }

            if (ConsoleHelper.ConfirmAction($"Вы уверены, что хотите удалить операцию на сумму {operation.Amount:C}?"))
            {
                _operationFacade.DeleteOperation(operationId);
                ConsoleHelper.PrintSuccess("Операция успешно удалена!");
            }
        }
    }
}