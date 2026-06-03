using HSE_Bank.Models;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Factories;

namespace HSE_Bank.Services.Import
{
    public class ImportManager
    {
        private readonly IBankAccountFacade _accountFacade;
        private readonly ICategoryFacade _categoryFacade;
        private readonly IOperationFacade _operationFacade;
        private readonly IBankAccountFactory _accountFactory;
        private readonly ICategoryFactory _categoryFactory;
        private readonly IOperationFactory _operationFactory;

        public ImportManager(
            IBankAccountFacade accountFacade, 
            ICategoryFacade categoryFacade, 
            IOperationFacade operationFacade,
            IBankAccountFactory accountFactory,
            ICategoryFactory categoryFactory,
            IOperationFactory operationFactory)
        {
            _accountFacade = accountFacade;
            _categoryFacade = categoryFacade;
            _operationFacade = operationFacade;
            _accountFactory = accountFactory;
            _categoryFactory = categoryFactory;
            _operationFactory = operationFactory;
        }

        public string ImportData(string filePath)
        {
            IDataImporter importer = GetImporter(filePath);
            
            if (importer == null)
                return "Неподдерживаемый формат файла";

            var result = importer.Import(filePath);
            
            if (!result.Success)
                return result.Message;

            // Сохраняем импортированные данные в основное хранилище
            SaveImportedData(result);
            
            return $"Импорт завершен: {result.Accounts.Count} счетов, {result.Categories.Count} категорий, {result.Operations.Count} операций. Данные сохранены в системе.";
        }

        private IDataImporter GetImporter(string filePath)
        {
            var extension = Path.GetExtension(filePath).ToLower();
            
            return extension switch
            {
                ".csv" => new CsvImporter(),
                ".json" => new JsonImporter(),
                ".yaml" or ".yml" => new YamlImporter(),
                _ => null
            };
        }

        private void SaveImportedData(ImportResult result)
        {
            try
            {
                Console.WriteLine("Сохранение импортированных данных в систему...");

                // Сохраняем счета
                foreach (var account in result.Accounts)
                {
                    var existingAccount = _accountFacade.GetAccount(account.Id);
                    if (existingAccount == null)
                    {
                        // Создаем новый счет через фабрику
                        var newAccount = _accountFactory.CreateAccount(account.Name, account.Balance);
                        // Присваиваем оригинальный ID (фабрика создает новые ID, поэтому нужно обновить)
                        newAccount.Id = account.Id;
                        // Добавляем в репозиторий
                        _accountFacade.CreateAccount(newAccount.Name, newAccount.Balance);
                        Console.WriteLine($"Создан счет: {account.Name}");
                    }
                    else
                    {
                        // Обновляем существующий счет
                        existingAccount.Name = account.Name;
                        existingAccount.Balance = account.Balance;
                        _accountFacade.UpdateAccount(existingAccount);
                        Console.WriteLine($"Обновлен счет: {account.Name}");
                    }
                }

                // Сохраняем категории
                foreach (var category in result.Categories)
                {
                    var existingCategory = _categoryFacade.GetCategory(category.Id);
                    if (existingCategory == null)
                    {
                        // Создаем новую категорию через фабрику
                        var newCategory = _categoryFactory.CreateCategory(category.Type, category.Name);
                        // Присваиваем оригинальный ID
                        newCategory.Id = category.Id;
                        // Добавляем в репозиторий
                        _categoryFacade.CreateCategory(newCategory.Type, newCategory.Name);
                        Console.WriteLine($"Создана категория: {category.Name}");
                    }
                    else
                    {
                        // Обновляем существующую категорию
                        existingCategory.Name = category.Name;
                        existingCategory.Type = category.Type;
                        _categoryFacade.UpdateCategory(existingCategory);
                        Console.WriteLine($"Обновлена категория: {category.Name}");
                    }
                }

                // Сохраняем операции
                foreach (var operation in result.Operations)
                {
                    var existingOperation = _operationFacade.GetOperation(operation.Id);
                    if (existingOperation == null)
                    {
                        // Создаем новую операцию через фабрику
                        var newOperation = _operationFactory.CreateOperation(
                            operation.Type,
                            operation.BankAccountId,
                            operation.Amount,
                            operation.Date,
                            operation.Description,
                            operation.CategoryId
                        );
                        // Присваиваем оригинальный ID
                        newOperation.Id = operation.Id;
                        // Добавляем в репозиторий (используем фасад, который обновит баланс счета)
                        _operationFacade.CreateOperation(
                            newOperation.Type,
                            newOperation.BankAccountId,
                            newOperation.Amount,
                            newOperation.Date,
                            newOperation.Description,
                            newOperation.CategoryId
                        );
                        Console.WriteLine($"Создана операция: {operation.Description}");
                    }
                    else
                    {
                        // Обновляем существующую операцию
                        existingOperation.Type = operation.Type;
                        existingOperation.BankAccountId = operation.BankAccountId;
                        existingOperation.Amount = operation.Amount;
                        existingOperation.Date = operation.Date;
                        existingOperation.Description = operation.Description;
                        existingOperation.CategoryId = operation.CategoryId;
                        _operationFacade.UpdateOperation(existingOperation);
                        Console.WriteLine($"Обновлена операция: {operation.Description}");
                    }
                }

                Console.WriteLine("Все данные успешно сохранены в системе!");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при сохранении данных: {ex.Message}");
                throw;
            }
        }
    }
}