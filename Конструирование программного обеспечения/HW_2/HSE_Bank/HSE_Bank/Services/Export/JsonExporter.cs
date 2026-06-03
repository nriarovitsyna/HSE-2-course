using System.Text.Json;

namespace HSE_Bank.Services.Export
{
    public class JsonExporter : DataExporter
    {
        protected override string FormatData(object data)
        {
            var exportData = data as ExportData;
            if (exportData == null)
                throw new Exception("Неверный формат данных для экспорта");

            var exportObject = new
            {
                accounts = exportData.Accounts,
                categories = exportData.Categories,
                operations = exportData.Operations
            };

            var options = new JsonSerializerOptions
            {
                WriteIndented = true,
                PropertyNamingPolicy = JsonNamingPolicy.CamelCase
            };

            return JsonSerializer.Serialize(exportObject, options);
        }
    }
}