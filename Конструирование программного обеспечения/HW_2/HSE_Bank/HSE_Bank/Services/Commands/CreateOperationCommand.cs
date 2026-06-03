using HSE_Bank.Models;
using HSE_Bank.Services.Facades;

namespace HSE_Bank.Services.Commands
{
    public class CreateOperationCommand : ICommand
    {
        private readonly IOperationFacade _facade;
        private readonly OperationType _type;
        private readonly int _accountId;
        private readonly decimal _amount;
        private readonly DateTime _date;
        private readonly string _description;
        private readonly int _categoryId;

        public CreateOperationCommand(IOperationFacade facade, OperationType type, int accountId, 
            decimal amount, DateTime date, string description, int categoryId)
        {
            _facade = facade;
            _type = type;
            _accountId = accountId;
            _amount = amount;
            _date = date;
            _description = description;
            _categoryId = categoryId;
        }

        public void Execute()
        {
            var operation = _facade.CreateOperation(_type, _accountId, _amount, _date, _description, _categoryId);
            Console.WriteLine($"Операция создана: {operation.Type} на сумму {operation.Amount:C}");
        }
    }
}