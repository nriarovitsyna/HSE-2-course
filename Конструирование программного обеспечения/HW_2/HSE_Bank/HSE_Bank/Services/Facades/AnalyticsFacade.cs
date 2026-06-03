using HSE_Bank.Models;

namespace HSE_Bank.Services.Facades
{
    public class AnalyticsFacade : IAnalyticsFacade
    {
        private readonly IOperationFacade _operationFacade;
        private readonly ICategoryFacade _categoryFacade;

        public AnalyticsFacade(IOperationFacade operationFacade, ICategoryFacade categoryFacade)
        {
            _operationFacade = operationFacade;
            _categoryFacade = categoryFacade;
        }

        public decimal GetBalanceDifference(DateTime startDate, DateTime endDate)
        {
            var operations = _operationFacade.GetOperationsByPeriod(startDate, endDate);
            return operations.Where(o => o.Type == OperationType.Income).Sum(o => o.Amount) -
                   operations.Where(o => o.Type == OperationType.Expense).Sum(o => o.Amount);
        }

        public Dictionary<string, decimal> GetGroupedOperationsByCategory(DateTime startDate, DateTime endDate)
        {
            var operations = _operationFacade.GetOperationsByPeriod(startDate, endDate);
            var categories = _categoryFacade.GetAllCategories();
            
            return operations.GroupBy(o => categories.First(c => c.Id == o.CategoryId).Name)
                           .ToDictionary(g => g.Key, g => g.Sum(o => o.Amount));
        }

        public Dictionary<string, decimal> GetIncomeByCategory(DateTime startDate, DateTime endDate)
        {
            var operations = _operationFacade.GetOperationsByPeriod(startDate, endDate)
                                           .Where(o => o.Type == OperationType.Income);
            var categories = _categoryFacade.GetAllCategories();
            
            return operations.GroupBy(o => categories.First(c => c.Id == o.CategoryId).Name)
                           .ToDictionary(g => g.Key, g => g.Sum(o => o.Amount));
        }

        public Dictionary<string, decimal> GetExpensesByCategory(DateTime startDate, DateTime endDate)
        {
            var operations = _operationFacade.GetOperationsByPeriod(startDate, endDate)
                                           .Where(o => o.Type == OperationType.Expense);
            var categories = _categoryFacade.GetAllCategories();
            
            return operations.GroupBy(o => categories.First(c => c.Id == o.CategoryId).Name)
                           .ToDictionary(g => g.Key, g => g.Sum(o => o.Amount));
        }
    }
}