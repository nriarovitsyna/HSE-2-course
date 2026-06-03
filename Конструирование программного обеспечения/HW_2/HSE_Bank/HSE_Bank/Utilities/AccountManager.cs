using HSE_Bank.Models;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Commands;

namespace HSE_Bank.Utilities
{
    public class AccountManager
    {
        private readonly IBankAccountFacade _facade;

        public AccountManager(IBankAccountFacade facade)
        {
            _facade = facade;
        }

        public void ManageAccounts()
        {
            while (true)
            {
                ConsoleHelper.PrintHeader("Управление счетами");

                var accounts = _facade.GetAllAccounts();
                ConsoleHelper.PrintAccounts(accounts);

                Console.WriteLine("\nДоступные действия:");
                Console.WriteLine("1. Создать новый счет");
                Console.WriteLine("2. Редактировать счет");
                Console.WriteLine("3. Удалить счет");
                Console.WriteLine("4. Назад в главное меню");
                Console.WriteLine(new string('-', 40));

                var choice = ConsoleHelper.ReadInt("Выберите действие");

                switch (choice)
                {
                    case 1: CreateAccount(); break;
                    case 2: EditAccount(accounts); break;
                    case 3: DeleteAccount(accounts); break;
                    case 4: return;
                    default: ConsoleHelper.PrintError("Неверный выбор"); break;
                }

                ConsoleHelper.WaitForContinue();
            }
        }

        private void CreateAccount()
        {
            ConsoleHelper.PrintHeader("Создание нового счета");

            var name = ConsoleHelper.ReadString("Название счета");
            var balance = ConsoleHelper.ReadDecimal("Начальный баланс");

            var command = new CreateAccountCommand(_facade, name, balance);
            var decoratedCommand = new CommandDecorator(command);
            decoratedCommand.Execute();
        }

        private void EditAccount(List<BankAccount> accounts)
        {
            if (!accounts.Any()) return;

            var accountId = ConsoleHelper.ReadInt("Введите ID счета для редактирования");
            var account = _facade.GetAccount(accountId);

            if (account == null)
            {
                ConsoleHelper.PrintError("Счет не найден");
                return;
            }

            Console.WriteLine($"\nРедактирование счета: {account.Name}");
            var newName = ConsoleHelper.ReadString($"Новое название (текущее: {account.Name})");
            var newBalance = ConsoleHelper.ReadDecimal($"Новый баланс (текущий: {account.Balance:C})");

            if (!string.IsNullOrWhiteSpace(newName))
                account.Name = newName;

            account.Balance = newBalance;
            _facade.UpdateAccount(account);

            ConsoleHelper.PrintSuccess("Счет успешно обновлен!");
        }

        private void DeleteAccount(List<BankAccount> accounts)
        {
            if (!accounts.Any()) return;

            var accountId = ConsoleHelper.ReadInt("Введите ID счета для удаления");
            var account = _facade.GetAccount(accountId);

            if (account == null)
            {
                ConsoleHelper.PrintError("Счет не найден");
                return;
            }

            if (ConsoleHelper.ConfirmAction($"Вы уверены, что хотите удалить счет '{account.Name}'?"))
            {
                _facade.DeleteAccount(accountId);
                ConsoleHelper.PrintSuccess("Счет успешно удален!");
            }
        }
    }
}