using HSE_Bank.Models;

namespace HSE_Bank.Services.Facades
{
    public interface IBankAccountFacade
    {
        BankAccount CreateAccount(string name, decimal initialBalance);
        BankAccount GetAccount(int id);
        List<BankAccount> GetAllAccounts();
        void UpdateAccount(BankAccount account);
        void DeleteAccount(int id);
    }
}