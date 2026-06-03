using HSE_Bank.Services.Facades;

namespace HSE_Bank.Services.Commands
{
    public class CreateAccountCommand : ICommand
    {
        private readonly IBankAccountFacade _facade;
        private readonly string _name;
        private readonly decimal _initialBalance;

        public CreateAccountCommand(IBankAccountFacade facade, string name, decimal initialBalance)
        {
            _facade = facade;
            _name = name;
            _initialBalance = initialBalance;
        }

        public void Execute()
        {
            var account = _facade.CreateAccount(_name, _initialBalance);
            Console.WriteLine($"Счет создан: {account.Name} (ID: {account.Id}), Баланс: {account.Balance:C}");
        }
    }
}