using HSE_Bank.Services.Facades;

namespace HSE_Bank.Services.Commands
{
    public class GetAnalyticsCommand : ICommand
    {
        private readonly IAnalyticsFacade _facade;
        private readonly DateTime _startDate;
        private readonly DateTime _endDate;

        public GetAnalyticsCommand(IAnalyticsFacade facade, DateTime startDate, DateTime endDate)
        {
            _facade = facade;
            _startDate = startDate;
            _endDate = endDate;
        }

        public void Execute()
        {
            Console.WriteLine($"\nАналитика за период: {_startDate:dd.MM.yyyy} - {_endDate:dd.MM.yyyy}");
            
            var balanceDiff = _facade.GetBalanceDifference(_startDate, _endDate);
            Console.WriteLine($"Разница доходов и расходов: {balanceDiff:C}");
            
            var incomeByCategory = _facade.GetIncomeByCategory(_startDate, _endDate);
            Console.WriteLine("\nДоходы по категориям:");
            foreach (var item in incomeByCategory)
            {
                Console.WriteLine($"  {item.Key}: {item.Value:C}");
            }
            
            var expensesByCategory = _facade.GetExpensesByCategory(_startDate, _endDate);
            Console.WriteLine("\nРасходы по категориям:");
            foreach (var item in expensesByCategory)
            {
                Console.WriteLine($"  {item.Key}: {item.Value:C}");
            }
        }
    }
}