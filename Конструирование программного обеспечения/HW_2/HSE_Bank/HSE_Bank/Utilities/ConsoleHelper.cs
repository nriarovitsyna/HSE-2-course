using HSE_Bank.Models;

namespace HSE_Bank.Utilities
{
    public static class ConsoleHelper
    {
        public static void PrintHeader(string title)
        {
            Console.Clear();
            Console.WriteLine($"{new string('=', 60)}");
            Console.WriteLine($"{title}");
            Console.WriteLine(new string('=', 60));
        }

        public static void PrintSuccess(string message)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"[УСПЕХ] {message}");
            Console.ResetColor();
        }

        public static void PrintError(string message)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"[ОШИБКА] {message}");
            Console.ResetColor();
        }

        public static void PrintWarning(string message)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine($"[ПРЕДУПРЕЖДЕНИЕ] {message}");
            Console.ResetColor();
        }

        public static void PrintInfo(string message)
        {
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine($"[ИНФО] {message}");
            Console.ResetColor();
        }

        public static string ReadString(string prompt)
        {
            Console.Write($"{prompt}: ");
            return Console.ReadLine()?.Trim() ?? "";
        }

        public static decimal ReadDecimal(string prompt)
        {
            while (true)
            {
                Console.Write($"{prompt}: ");
                if (decimal.TryParse(Console.ReadLine(), out decimal result) && result >= 0)
                    return result;
                PrintError("Введите корректное неотрицательное число");
            }
        }

        public static int ReadInt(string prompt)
        {
            while (true)
            {
                Console.Write($"{prompt}: ");
                if (int.TryParse(Console.ReadLine(), out int result) && result >= 0)
                    return result;
                PrintError("Введите корректное целое неотрицательное число");
            }
        }

        public static DateTime ReadDate(string prompt)
        {
            while (true)
            {
                Console.Write($"{prompt} (дд.мм.гггг): ");
                if (DateTime.TryParse(Console.ReadLine(), out DateTime result) && result <= DateTime.Now)
                    return result;
                PrintError("Введите корректную дату (не будущую)");
            }
        }

        public static OperationType ReadOperationType()
        {
            while (true)
            {
                Console.WriteLine("\nВыберите тип операции:");
                Console.WriteLine("1. Доход");
                Console.WriteLine("2. Расход");
                Console.Write("Ваш выбор (1-2): ");
                
                var choice = Console.ReadLine();
                switch (choice)
                {
                    case "1": return OperationType.Income;
                    case "2": return OperationType.Expense;
                    default: PrintError("Выберите 1 или 2"); break;
                }
            }
        }

        public static void WaitForContinue()
        {
            Console.WriteLine("\nНажмите любую клавишу для продолжения...");
            Console.ReadKey();
        }

        public static void PrintAccounts(List<BankAccount> accounts)
        {
            if (!accounts.Any())
            {
                PrintWarning("Счета не найдены");
                return;
            }

            Console.WriteLine("\nСписок счетов:");
            Console.WriteLine(new string('-', 50));
            foreach (var account in accounts)
            {
                Console.WriteLine($"ID: {account.Id} | Название: {account.Name} | Баланс: {account.Balance:C}");
            }
        }

        public static void PrintCategories(List<Category> categories)
        {
            if (!categories.Any())
            {
                PrintWarning("Категории не найдены");
                return;
            }

            var incomeCategories = categories.Where(c => c.Type == OperationType.Income).ToList();
            var expenseCategories = categories.Where(c => c.Type == OperationType.Expense).ToList();

            if (incomeCategories.Any())
            {
                Console.WriteLine("\nКатегории доходов:");
                Console.WriteLine(new string('-', 30));
                foreach (var category in incomeCategories)
                {
                    Console.WriteLine($"ID: {category.Id} | Название: {category.Name}");
                }
            }

            if (expenseCategories.Any())
            {
                Console.WriteLine("\nКатегории расходов:");
                Console.WriteLine(new string('-', 30));
                foreach (var category in expenseCategories)
                {
                    Console.WriteLine($"ID: {category.Id} | Название: {category.Name}");
                }
            }
        }

        public static void PrintOperations(List<Operation> operations, List<Category> categories, List<BankAccount> accounts)
        {
            if (!operations.Any())
            {
                PrintWarning("Операции не найдены");
                return;
            }

            Console.WriteLine("\nСписок операций:");
            Console.WriteLine(new string('-', 80));
            
            foreach (var operation in operations.OrderByDescending(o => o.Date))
            {
                var category = categories.FirstOrDefault(c => c.Id == operation.CategoryId);
                var account = accounts.FirstOrDefault(a => a.Id == operation.BankAccountId);
                
                var typeText = operation.Type == OperationType.Income ? "Доход" : "Расход";
                var amountSign = operation.Type == OperationType.Income ? "+" : "-";
                
                Console.WriteLine($"{typeText} | ID: {operation.Id} | Сумма: {amountSign}{operation.Amount:C} | " +
                                $"Дата: {operation.Date:dd.MM.yyyy} | Категория: {category?.Name ?? "N/A"} | " +
                                $"Счет: {account?.Name ?? "N/A"} | Описание: {operation.Description}");
            }
        }

        public static bool ConfirmAction(string message)
        {
            Console.Write($"\n{message} (y/n): ");
            var response = Console.ReadLine()?.Trim().ToLower();
            return response == "y" || response == "yes" || response == "д" || response == "да";
        }
    }
}