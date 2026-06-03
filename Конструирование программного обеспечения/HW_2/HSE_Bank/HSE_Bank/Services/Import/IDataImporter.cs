using HSE_Bank.Models;

namespace HSE_Bank.Services.Import
{
    public interface IDataImporter
    {
        ImportResult Import(string filePath);
    }

    public class ImportResult
    {
        public bool Success { get; set; }
        public string Message { get; set; }
        public List<BankAccount> Accounts { get; set; } = new();
        public List<Category> Categories { get; set; } = new();
        public List<Operation> Operations { get; set; } = new();
    }
}