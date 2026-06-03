using HSE_Bank.Models;

namespace HSE_Bank.Services.Factories
{
    public interface IBankAccountFactory
    {
        BankAccount CreateAccount(string name, decimal initialBalance);
    }
}