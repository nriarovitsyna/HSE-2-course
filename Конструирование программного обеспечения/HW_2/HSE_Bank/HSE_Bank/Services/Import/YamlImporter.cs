using HSE_Bank.Models;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace HSE_Bank.Services.Import
{
    public class YamlImporter : DataImporter
    {
        protected override void ParseData(string content, ImportResult result)
        {
            try
            {
                Console.WriteLine("Начало парсинга YAML...");

                var deserializer = new DeserializerBuilder()
                    .WithNamingConvention(CamelCaseNamingConvention.Instance)
                    .IgnoreUnmatchedProperties()
                    .Build();

                var yamlData = deserializer.Deserialize<YamlData>(content);

                if (yamlData == null)
                    throw new Exception("Неверный формат YAML");

                // Преобразуем данные в наши модели
                if (yamlData.Accounts != null)
                {
                    foreach (var accountData in yamlData.Accounts)
                    {
                        var account = new BankAccount(
                            accountData.Id,
                            accountData.Name,
                            accountData.Balance
                        );
                        result.Accounts.Add(account);
                        Console.WriteLine($"Добавлен счет: {account.Name}");
                    }
                }

                if (yamlData.Categories != null)
                {
                    foreach (var categoryData in yamlData.Categories)
                    {
                        var category = new Category(
                            categoryData.Id,
                            categoryData.Type,
                            categoryData.Name
                        );
                        result.Categories.Add(category);
                        Console.WriteLine($"Добавлена категория: {category.Name}");
                    }
                }

                if (yamlData.Operations != null)
                {
                    foreach (var operationData in yamlData.Operations)
                    {
                        var operation = new Operation(
                            operationData.Id,
                            operationData.Type,
                            operationData.BankAccountId,
                            operationData.Amount,
                            operationData.Date,
                            operationData.Description ?? "",
                            operationData.CategoryId
                        );
                        result.Operations.Add(operation);
                        Console.WriteLine($"Добавлена операция: {operation.Type} на сумму {operation.Amount}");
                    }
                }

                Console.WriteLine($"Парсинг завершен: {result.Accounts.Count} счетов, {result.Categories.Count} категорий, {result.Operations.Count} операций");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка парсинга YAML: {ex.Message}");
                throw new Exception($"Ошибка парсинга YAML: {ex.Message}");
            }
        }

        // Классы для десериализации YAML
        public class YamlAccount
        {
            public int Id { get; set; }
            public string Name { get; set; } = "";
            public decimal Balance { get; set; }
        }

        public class YamlCategory
        {
            public int Id { get; set; }
            public OperationType Type { get; set; }
            public string Name { get; set; } = "";
        }

        public class YamlOperation
        {
            public int Id { get; set; }
            public OperationType Type { get; set; }
            public int BankAccountId { get; set; }
            public decimal Amount { get; set; }
            public DateTime Date { get; set; }
            public string? Description { get; set; }
            public int CategoryId { get; set; }
        }

        public class YamlData
        {
            public List<YamlAccount> Accounts { get; set; } = new();
            public List<YamlCategory> Categories { get; set; } = new();
            public List<YamlOperation> Operations { get; set; } = new();
        }
    }
}