using HSE_Bank.Models;
using HSE_Bank.Repository;
using HSE_Bank.Services.Factories;

namespace HSE_Bank.Services.Facades
{
    public class OperationFacade : IOperationFacade
    {
        private readonly IRepository<Operation> _repository;
        private readonly IOperationFactory _factory;
        private readonly IBankAccountFacade _accountFacade;

        public OperationFacade(IRepository<Operation> repository, IOperationFactory factory, 
                             IBankAccountFacade accountFacade)
        {
            _repository = repository;
            _factory = factory;
            _accountFacade = accountFacade;
        }

        public Operation CreateOperation(OperationType type, int bankAccountId, decimal amount, 
                                       DateTime date, string description, int categoryId)
        {
            var operation = _factory.CreateOperation(type, bankAccountId, amount, date, description, categoryId);
            
            // Update account balance
            var account = _accountFacade.GetAccount(bankAccountId);
            if (account != null)
            {
                account.Balance += type == OperationType.Income ? amount : -amount;
                _accountFacade.UpdateAccount(account);
            }

            _repository.Add(operation);
            return operation;
        }

        public Operation GetOperation(int id) => _repository.GetById(id);
        public List<Operation> GetAllOperations() => _repository.GetAll();

        public List<Operation> GetOperationsByAccount(int bankAccountId)
            => _repository.GetAll().Where(o => o.BankAccountId == bankAccountId).ToList();

        public List<Operation> GetOperationsByPeriod(DateTime startDate, DateTime endDate)
            => _repository.GetAll().Where(o => o.Date >= startDate && o.Date <= endDate).ToList();

        public void UpdateOperation(Operation operation) => _repository.Update(operation);
        public void DeleteOperation(int id) => _repository.Delete(id);
    }
}