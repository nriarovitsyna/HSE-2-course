using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public interface IOperationFactory
    {
        Operation CreateOperation(OperationType type, int bankAccountId, decimal amount, 
            DateTime date, string description, int categoryId);
    }
}