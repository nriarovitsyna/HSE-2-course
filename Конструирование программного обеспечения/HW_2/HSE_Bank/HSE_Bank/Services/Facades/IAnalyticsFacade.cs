namespace HSE_Bank.Services.Facades
{
    public interface IAnalyticsFacade
    {
        decimal GetBalanceDifference(DateTime startDate, DateTime endDate);
        Dictionary<string, decimal> GetGroupedOperationsByCategory(DateTime startDate, DateTime endDate);
        Dictionary<string, decimal> GetIncomeByCategory(DateTime startDate, DateTime endDate);
        Dictionary<string, decimal> GetExpensesByCategory(DateTime startDate, DateTime endDate);
    }
}