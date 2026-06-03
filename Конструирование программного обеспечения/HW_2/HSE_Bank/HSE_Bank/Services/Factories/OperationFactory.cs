using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public class OperationFactory : IOperationFactory
    {
        private static int _nextId = 1;

        public Operation CreateOperation(OperationType type, int bankAccountId, decimal amount, 
            DateTime date, string description, int categoryId)
        {
            if (amount <= 0)
                throw new ArgumentException("Сумма операции должна быть положительной");

            if (date > DateTime.Now)
                throw new ArgumentException("Дата операции не может быть в будущем");

            return new Operation(_nextId++, type, bankAccountId, amount, date, description ?? "", categoryId);
        }
    }
}