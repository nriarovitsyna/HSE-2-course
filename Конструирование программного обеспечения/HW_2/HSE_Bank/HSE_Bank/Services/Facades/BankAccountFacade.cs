using HSE_Bank.Models;
using HSE_Bank.Repository;
using HSE_Bank.Services.Factories;

namespace HSE_Bank.Services.Facades
{
    public class BankAccountFacade : IBankAccountFacade
    {
        private readonly IRepository<BankAccount> _repository;
        private readonly IBankAccountFactory _factory;

        public BankAccountFacade(IRepository<BankAccount> repository, IBankAccountFactory factory)
        {
            _repository = repository;
            _factory = factory;
        }

        public BankAccount CreateAccount(string name, decimal initialBalance)
        {
            var account = _factory.CreateAccount(name, initialBalance);
            _repository.Add(account);
            return account;
        }

        public BankAccount GetAccount(int id) => _repository.GetById(id);
        public List<BankAccount> GetAllAccounts() => _repository.GetAll();
        
        public void UpdateAccount(BankAccount account) => _repository.Update(account);
        public void DeleteAccount(int id) => _repository.Delete(id);
    }
}