using HSE_Bank.Services.Facades;

namespace HSE_Bank.Services.Export
{
    public class ExportManager
    {
        private readonly IBankAccountFacade _accountFacade;
        private readonly ICategoryFacade _categoryFacade;
        private readonly IOperationFacade _operationFacade;

        public ExportManager(IBankAccountFacade accountFacade, ICategoryFacade categoryFacade, IOperationFacade operationFacade)
        {
            _accountFacade = accountFacade;
            _categoryFacade = categoryFacade;
            _operationFacade = operationFacade;
        }

        public ExportResult ExportData(string filePath)
        {
            IDataExporter exporter = GetExporter(filePath);
            
            if (exporter == null)
                return new ExportResult { Success = false, Message = "Неподдерживаемый формат файла" };

            var data = CollectData();
            return exporter.Export(filePath, data);
        }

        private IDataExporter GetExporter(string filePath)
        {
            var extension = Path.GetExtension(filePath).ToLower();
            
            return extension switch
            {
                ".csv" => new CsvExporter(),
                ".json" => new JsonExporter(),
                ".yaml" or ".yml" => new YamlExporter(),
                _ => null
            };
        }

        private ExportData CollectData()
        {
            return new ExportData
            {
                Accounts = _accountFacade.GetAllAccounts(),
                Categories = _categoryFacade.GetAllCategories(),
                Operations = _operationFacade.GetAllOperations()
            };
        }
    }
}