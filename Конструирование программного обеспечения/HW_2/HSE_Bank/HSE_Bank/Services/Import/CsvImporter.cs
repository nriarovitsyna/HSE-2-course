using System.Text;
using HSE_Bank.Models;

namespace HSE_Bank.Services.Import
{
    public class CsvImporter : DataImporter
    {
        protected override void ParseData(string content, ImportResult result)
        {
            try
            {
                Console.WriteLine("Начало парсинга CSV...");
                
                var lines = content.Split('\n')
                    .Select(line => line.Trim())
                    .Where(line => !string.IsNullOrWhiteSpace(line))
                    .ToArray();

                if (lines.Length == 0)
                    throw new Exception("CSV файл пуст");
                
                int startIndex = lines[0].StartsWith("Type,Id,Name,Balance") ? 1 : 0;

                for (int i = startIndex; i < lines.Length; i++)
                {
                    try
                    {
                        var line = lines[i];
                        var values = ParseCsvLine(line);
                        
                        if (values.Length < 2) continue;

                        ParseLine(values, result);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Ошибка в строке {i + 1}: {lines[i]} - {ex.Message}");
                    }
                }

                Console.WriteLine($"Парсинг завершен: {result.Accounts.Count} счетов, {result.Categories.Count} категорий, {result.Operations.Count} операций");
            }
            catch (Exception ex)
            {
                throw new Exception($"Ошибка парсинга CSV: {ex.Message}");
            }
        }

        private string[] ParseCsvLine(string line)
        {
            var result = new List<string>();
            var inQuotes = false;
            var currentField = new StringBuilder();

            for (int i = 0; i < line.Length; i++)
            {
                char c = line[i];

                if (c == '"')
                {
                    if (inQuotes && i + 1 < line.Length && line[i + 1] == '"')
                    {
                        currentField.Append('"');
                        i++;
                    }
                    else
                    {
                        inQuotes = !inQuotes;
                    }
                }
                else if (c == ',' && !inQuotes)
                {
                    result.Add(currentField.ToString());
                    currentField.Clear();
                }
                else
                {
                    currentField.Append(c);
                }
            }

            result.Add(currentField.ToString());
            return result.Select(f => f.Trim()).ToArray();
        }

        private void ParseLine(string[] values, ImportResult result)
        {
            var type = values[0].ToLower();

            switch (type)
            {
                case "account":
                    ParseAccount(values, result);
                    break;
                case "category":
                    ParseCategory(values, result);
                    break;
                case "operation":
                    ParseOperation(values, result);
                    break;
                default:
                    Console.WriteLine($"Неизвестный тип записи: {type}");
                    break;
            }
        }

        private void ParseAccount(string[] values, ImportResult result)
        {
            // Format: Account,Id,Name,Balance,OperationType,BankAccountId,Amount,Date,Description,CategoryId
            if (values.Length >= 4)
            {
                var account = new BankAccount(
                    int.Parse(values[1]), // Id
                    values[2],           // Name
                    decimal.Parse(values[3]) // Balance
                );
                result.Accounts.Add(account);
                Console.WriteLine($"Добавлен счет: {account.Name}");
            }
        }

        private void ParseCategory(string[] values, ImportResult result)
        {
            // Format: Category,Id,Name,Type,OperationType,BankAccountId,Amount,Date,Description,CategoryId
            if (values.Length >= 4)
            {
                var category = new Category(
                    int.Parse(values[1]), // Id
                    ParseOperationType(values[3]), // Type (4-я колонка)
                    values[2] // Name (3-я колонка)
                );
                result.Categories.Add(category);
                Console.WriteLine($"Добавлена категория: {category.Name}");
            }
        }

        private void ParseOperation(string[] values, ImportResult result)
        {
            // Format: Operation,Id,Description,Type,BankAccountId,Amount,Date,CategoryId
            if (values.Length >= 8)
            {
                var operation = new Operation(
                    int.Parse(values[1]),        // Id
                    ParseOperationType(values[3]), // Type (4-я колонка)
                    int.Parse(values[4]),       // BankAccountId (5-я колонка)
                    decimal.Parse(values[5]),   // Amount (6-я колонка)
                    ParseDateTime(values[6]),   // Date (7-я колонка)
                    values[2],                  // Description (3-я колонка)
                    int.Parse(values[7])        // CategoryId (8-я колонка)
                );
                result.Operations.Add(operation);
                Console.WriteLine($"Добавлена операция: {operation.Description}");
            }
            else
            {
                Console.WriteLine($"Недостаточно данных для операции. Ожидается 8 полей, получено: {values.Length}");
                Console.WriteLine($"Данные: {string.Join(" | ", values)}");
            }
        }

        private OperationType ParseOperationType(string value)
        {
            if (string.IsNullOrEmpty(value)) 
                throw new FormatException("Тип операции не может быть пустым");

            if (Enum.TryParse<OperationType>(value, true, out var result))
                return result;

            if (value.Equals("доход", StringComparison.OrdinalIgnoreCase))
                return OperationType.Income;

            if (value.Equals("расход", StringComparison.OrdinalIgnoreCase))
                return OperationType.Expense;

            throw new FormatException($"Неизвестный тип операции: '{value}'");
        }

        private DateTime ParseDateTime(string value)
        {
            if (DateTime.TryParse(value, out DateTime result))
                return result;
            
            string[] formats = {
                "yyyy-MM-dd",
                "dd.MM.yyyy",
                "MM/dd/yyyy",
                "yyyy/MM/dd"
            };

            if (DateTime.TryParseExact(value, formats, System.Globalization.CultureInfo.InvariantCulture,
                System.Globalization.DateTimeStyles.None, out result))
                return result;

            throw new FormatException($"Не удалось распознать дату: '{value}'");
        }
    }
}