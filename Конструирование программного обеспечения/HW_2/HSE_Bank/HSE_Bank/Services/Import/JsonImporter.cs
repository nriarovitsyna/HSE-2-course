using System.Text.Json;
using HSE_Bank.Models;

namespace HSE_Bank.Services.Import
{
    public class JsonImporter : DataImporter
    {
        protected override void ParseData(string content, ImportResult result)
        {
            try
            {
                Console.WriteLine("Начало парсинга JSON...");
                
                using JsonDocument doc = JsonDocument.Parse(content);
                var root = doc.RootElement;

                // Парсим счета
                if (root.TryGetProperty("accounts", out var accountsElement))
                {
                    foreach (var accountElement in accountsElement.EnumerateArray())
                    {
                        var account = new BankAccount(
                            accountElement.GetProperty("id").GetInt32(),
                            accountElement.GetProperty("name").GetString() ?? "",
                            accountElement.GetProperty("balance").GetDecimal()
                        );
                        result.Accounts.Add(account);
                        Console.WriteLine($"Добавлен счет: {account.Name}");
                    }
                }

                // Парсим категории
                if (root.TryGetProperty("categories", out var categoriesElement))
                {
                    foreach (var categoryElement in categoriesElement.EnumerateArray())
                    {
                        var typeValue = categoryElement.GetProperty("type");
                        OperationType type;
                        
                        if (typeValue.ValueKind == JsonValueKind.String)
                        {
                            var typeString = typeValue.GetString() ?? "";
                            type = ParseOperationType(typeString);
                        }
                        else
                        {
                            type = (OperationType)typeValue.GetInt32();
                        }
                        
                        var category = new Category(
                            categoryElement.GetProperty("id").GetInt32(),
                            type,
                            categoryElement.GetProperty("name").GetString() ?? ""
                        );
                        result.Categories.Add(category);
                        Console.WriteLine($"Добавлена категория: {category.Name}");
                    }
                }

                // Парсим операции
                if (root.TryGetProperty("operations", out var operationsElement))
                {
                    foreach (var operationElement in operationsElement.EnumerateArray())
                    {
                        var typeValue = operationElement.GetProperty("type");
                        OperationType type;
                        
                        if (typeValue.ValueKind == JsonValueKind.String)
                        {
                            var typeString = typeValue.GetString() ?? "";
                            type = ParseOperationType(typeString);
                        }
                        else
                        {
                            type = (OperationType)typeValue.GetInt32();
                        }
                        
                        var operation = new Operation(
                            operationElement.GetProperty("id").GetInt32(),
                            type,
                            operationElement.GetProperty("bankAccountId").GetInt32(),
                            operationElement.GetProperty("amount").GetDecimal(),
                            operationElement.GetProperty("date").GetDateTime(),
                            operationElement.GetProperty("description").GetString() ?? "",
                            operationElement.GetProperty("categoryId").GetInt32()
                        );
                        result.Operations.Add(operation);
                        Console.WriteLine($"Добавлена операция: {operation.Type} на сумму {operation.Amount}");
                    }
                }

                Console.WriteLine($"Парсинг завершен: {result.Accounts.Count} счетов, {result.Categories.Count} категорий, {result.Operations.Count} операций");
            }
            catch (Exception ex)
            {
                throw new Exception($"Ошибка парсинга JSON: {ex.Message}");
            }
        }

        private OperationType ParseOperationType(string value)
        {
            if (Enum.TryParse<OperationType>(value, true, out var result))
                return result;

            if (value.Equals("доход", StringComparison.OrdinalIgnoreCase))
                return OperationType.Income;

            if (value.Equals("расход", StringComparison.OrdinalIgnoreCase))
                return OperationType.Expense;

            throw new FormatException($"Неизвестный тип операции: '{value}'");
        }
    }
}