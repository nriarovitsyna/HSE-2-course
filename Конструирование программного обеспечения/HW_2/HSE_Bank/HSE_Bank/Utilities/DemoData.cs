using HSE_Bank.Models;
using HSE_Bank.Services.Facades;

namespace HSE_Bank.Utilities
{
    public class DemoData
    {
        private readonly ICategoryFacade _categoryFacade;
        private readonly IBankAccountFacade _accountFacade;
        private readonly IOperationFacade _operationFacade;

        public DemoData(ICategoryFacade categoryFacade, IBankAccountFacade accountFacade, IOperationFacade operationFacade)
        {
            _categoryFacade = categoryFacade;
            _accountFacade = accountFacade;
            _operationFacade = operationFacade;
        }

        public void InitializeSampleData()
        {
            // Создаем примеры категорий
            _categoryFacade.CreateCategory(OperationType.Income, "Зарплата");
            _categoryFacade.CreateCategory(OperationType.Income, "Кэшбэк");
            _categoryFacade.CreateCategory(OperationType.Income, "Инвестиции");
            _categoryFacade.CreateCategory(OperationType.Expense, "Продукты");
            _categoryFacade.CreateCategory(OperationType.Expense, "Транспорт");
            _categoryFacade.CreateCategory(OperationType.Expense, "Развлечения");
            _categoryFacade.CreateCategory(OperationType.Expense, "ЖКХ");

            // Создаем примеры счетов
            _accountFacade.CreateAccount("Основной счет", 50000);
            _accountFacade.CreateAccount("Накопительный счет", 150000);
            _accountFacade.CreateAccount("Кредитная карта", 5000);

            // Создаем примеры операций
            var accounts = _accountFacade.GetAllAccounts();
            var categories = _categoryFacade.GetAllCategories();

            if (accounts.Any() && categories.Any())
            {
                var mainAccount = accounts[0];
                var salaryCategory = categories.First(c => c.Name == "Зарплата");
                var foodCategory = categories.First(c => c.Name == "Продукты");
                var transportCategory = categories.First(c => c.Name == "Транспорт");

                _operationFacade.CreateOperation(OperationType.Income, mainAccount.Id, 75000,
                    DateTime.Now.AddDays(-10), "Зарплата за январь", salaryCategory.Id);

                _operationFacade.CreateOperation(OperationType.Expense, mainAccount.Id, 5000,
                    DateTime.Now.AddDays(-5), "Продукты в Пятерочке", foodCategory.Id);

                _operationFacade.CreateOperation(OperationType.Expense, mainAccount.Id, 1500,
                    DateTime.Now.AddDays(-3), "Метро и такси", transportCategory.Id);
            }

            ConsoleHelper.PrintSuccess("Демонстрационные данные загружены!");
            ConsoleHelper.WaitForContinue();
        }
    }
}