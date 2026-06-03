namespace HSE_Bank.Services.Export
{
    public class CsvExporter : DataExporter
    {
        protected override string FormatData(object data)
        {
            var exportData = data as ExportData;
            if (exportData == null)
                throw new Exception("Неверный формат данных для экспорта");

            var lines = new List<string>();
            
            // Заголовки для CSV
            lines.Add("Type,Id,Name,Balance,OperationType,BankAccountId,Amount,Date,Description,CategoryId");
            
            // Счета
            foreach (var account in exportData.Accounts)
            {
                lines.Add($"Account,{account.Id},{EscapeCsv(account.Name)},{account.Balance},,,,");
            }
            
            // Категории
            foreach (var category in exportData.Categories)
            {
                lines.Add($"Category,{category.Id},{EscapeCsv(category.Name)},{category.Type},,,,");
            }
            
            // Операции
            foreach (var operation in exportData.Operations)
            {
                lines.Add($"Operation,{operation.Id},{EscapeCsv(operation.Description)},{operation.Type},{operation.BankAccountId},{operation.Amount},{operation.Date:yyyy-MM-dd},{operation.CategoryId}");
            }

            return string.Join("\n", lines);
        }

        private string EscapeCsv(string value)
        {
            if (string.IsNullOrEmpty(value)) return "";
            if (value.Contains(",") || value.Contains("\"") || value.Contains("\n"))
                return $"\"{value.Replace("\"", "\"\"")}\"";
            return value;
        }
    }
}