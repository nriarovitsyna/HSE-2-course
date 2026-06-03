using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace HSE_Bank.Services.Export
{
    public class YamlExporter : DataExporter
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

            var serializer = new SerializerBuilder()
                .WithNamingConvention(CamelCaseNamingConvention.Instance)
                .ConfigureDefaultValuesHandling(DefaultValuesHandling.OmitNull)
                .Build();

            return serializer.Serialize(exportObject);
        }
    }
}