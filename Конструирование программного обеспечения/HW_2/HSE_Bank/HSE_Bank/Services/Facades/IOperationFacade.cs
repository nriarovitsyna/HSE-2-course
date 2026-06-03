using HSE_Bank.Models;

namespace HSE_Bank.Services.Facades
{
    public interface IOperationFacade
    {
        Operation CreateOperation(OperationType type, int bankAccountId, decimal amount, 
            DateTime date, string description, int categoryId);
        Operation GetOperation(int id);
        List<Operation> GetAllOperations();
        List<Operation> GetOperationsByAccount(int bankAccountId);
        List<Operation> GetOperationsByPeriod(DateTime startDate, DateTime endDate);
        void UpdateOperation(Operation operation);
        void DeleteOperation(int id);
    }
}