using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Import;
using HSE_Bank.Services.Export;

namespace HSE_Bank.Utilities
{
    public class MenuManager
    {
        private readonly IBankAccountFacade _accountFacade;
        private readonly ICategoryFacade _categoryFacade;
        private readonly IOperationFacade _operationFacade;
        private readonly IAnalyticsFacade _analyticsFacade;
        private readonly ImportManager _importManager;
        private readonly ExportManager _exportManager;

        public MenuManager(
            IBankAccountFacade accountFacade,
            ICategoryFacade categoryFacade,
            IOperationFacade operationFacade,
            IAnalyticsFacade analyticsFacade,
            ImportManager importManager,
            ExportManager exportManager)
        {
            _accountFacade = accountFacade;
            _categoryFacade = categoryFacade;
            _operationFacade = operationFacade;
            _analyticsFacade = analyticsFacade;
            _importManager = importManager;
            _exportManager = exportManager;
        }

        public void RunMainMenu()
        {
            while (true)
            {
                ConsoleHelper.PrintHeader("HSE Bank - Главное меню");
                Console.WriteLine("1. Управление счетами");
                Console.WriteLine("2. Управление категориями");
                Console.WriteLine("3. Управление операциями");
                Console.WriteLine("4. Аналитика и отчеты");
                Console.WriteLine("5. Импорт данных");
                Console.WriteLine("6. Экспорт данных");
                Console.WriteLine("7. Выход");
                Console.WriteLine(new string('-', 40));

                var choice = ConsoleHelper.ReadInt("Выберите пункт меню");

                switch (choice)
                {
                    case 1: ManageAccounts(); break;
                    case 2: ManageCategories(); break;
                    case 3: ManageOperations(); break;
                    case 4: ShowAnalytics(); break;
                    case 5: ImportData(); break;
                    case 6: ExportData(); break;
                    case 7:
                        ConsoleHelper.PrintInfo("До свидания!");
                        return;
                    default:
                        ConsoleHelper.PrintError("Неверный выбор");
                        ConsoleHelper.WaitForContinue();
                        break;
                }
            }
        }

        private void ManageAccounts()
        {
            var accountManager = new AccountManager(_accountFacade);
            accountManager.ManageAccounts();
        }

        private void ManageCategories()
        {
            var categoryManager = new CategoryManager(_categoryFacade);
            categoryManager.ManageCategories();
        }

        private void ManageOperations()
        {
            var operationManager = new OperationManager(_operationFacade, _accountFacade, _categoryFacade);
            operationManager.ManageOperations();
        }

        private void ShowAnalytics()
        {
            var analyticsManager = new AnalyticsManager(_analyticsFacade, _operationFacade);
            analyticsManager.ShowAnalytics();
        }

        private void ImportData()
        {
            ConsoleHelper.PrintHeader("Импорт данных");
            Console.WriteLine("Поддерживаемые форматы: CSV, JSON, YAML");
            Console.WriteLine(new string('-', 40));

            var filePath = ConsoleHelper.ReadString("Путь к файлу для импорта");

            if (string.IsNullOrWhiteSpace(filePath))
            {
                ConsoleHelper.PrintError("Путь к файлу не может быть пустым");
                ConsoleHelper.WaitForContinue();
                return;
            }

            if (!File.Exists(filePath))
            {
                ConsoleHelper.PrintError("Файл не существует");
                ConsoleHelper.WaitForContinue();
                return;
            }

            var result = _importManager.ImportData(filePath);

            if (result.Contains("успешно") || result.Contains("завершен"))
                ConsoleHelper.PrintSuccess(result);
            else
                ConsoleHelper.PrintError(result);

            ConsoleHelper.WaitForContinue();
        }

        private void ExportData()
        {
            ConsoleHelper.PrintHeader("Экспорт данных");
            Console.WriteLine("Поддерживаемые форматы: CSV, JSON, YAML");
            Console.WriteLine(new string('-', 40));

            var filePath = ConsoleHelper.ReadString("Путь для сохранения файла");

            if (string.IsNullOrWhiteSpace(filePath))
            {
                ConsoleHelper.PrintError("Путь для экспорта не может быть пустым");
                ConsoleHelper.WaitForContinue();
                return;
            }

            var result = _exportManager.ExportData(filePath);

            if (result.Success)
            {
                ConsoleHelper.PrintSuccess(result.Message);
                ConsoleHelper.PrintInfo($"Файл сохранен: {result.FilePath}");
            }
            else
            {
                ConsoleHelper.PrintError(result.Message);
            }

            ConsoleHelper.WaitForContinue();
        }
    }
}