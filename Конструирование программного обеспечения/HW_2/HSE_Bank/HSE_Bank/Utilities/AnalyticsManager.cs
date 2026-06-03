using HSE_Bank.Models;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Commands;

namespace HSE_Bank.Utilities
{
    public class AnalyticsManager
    {
        private readonly IAnalyticsFacade _analyticsFacade;
        private readonly IOperationFacade _operationFacade;

        public AnalyticsManager(IAnalyticsFacade analyticsFacade, IOperationFacade operationFacade)
        {
            _analyticsFacade = analyticsFacade;
            _operationFacade = operationFacade;
        }

        public void ShowAnalytics()
        {
            ConsoleHelper.PrintHeader("Аналитика и отчеты");

            var startDate = ConsoleHelper.ReadDate("Начальная дата периода");
            var endDate = ConsoleHelper.ReadDate("Конечная дата периода");

            if (startDate > endDate)
            {
                ConsoleHelper.PrintError("Начальная дата не может быть больше конечной");
                ConsoleHelper.WaitForContinue();
                return;
            }

            var operations = _operationFacade.GetOperationsByPeriod(startDate, endDate);
            if (!operations.Any())
            {
                ConsoleHelper.PrintWarning("Нет операций за выбранный период");
                ConsoleHelper.WaitForContinue();
                return;
            }

            var command = new GetAnalyticsCommand(_analyticsFacade, startDate, endDate);
            var decoratedCommand = new CommandDecorator(command);
            decoratedCommand.Execute();

            // Дополнительная статистика
            Console.WriteLine("\nДополнительная статистика:");
            Console.WriteLine(new string('-', 40));
            Console.WriteLine($"Всего операций: {operations.Count}");
            Console.WriteLine($"Доходов: {operations.Count(o => o.Type == OperationType.Income)}");
            Console.WriteLine($"Расходов: {operations.Count(o => o.Type == OperationType.Expense)}");
            Console.WriteLine($"Период: {startDate:dd.MM.yyyy} - {endDate:dd.MM.yyyy}");

            ConsoleHelper.WaitForContinue();
        }
    }
}