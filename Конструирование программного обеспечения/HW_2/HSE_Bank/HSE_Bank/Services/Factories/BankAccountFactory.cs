using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public class BankAccountFactory : IBankAccountFactory
    {
        private static int _nextId = 1;

        public BankAccount CreateAccount(string name, decimal initialBalance)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Название счета не может быть пустым");
            
            if (initialBalance < 0)
                throw new ArgumentException("Начальный баланс не может быть отрицательным");

            return new BankAccount(_nextId++, name, initialBalance);
        }
    }
}