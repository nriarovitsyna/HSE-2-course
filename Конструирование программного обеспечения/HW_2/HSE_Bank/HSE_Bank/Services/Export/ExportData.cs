using HSE_Bank.Models;

namespace HSE_Bank.Services.Export
{
    public class ExportData
    {
        public List<BankAccount> Accounts { get; set; } = new();
        public List<Category> Categories { get; set; } = new();
        public List<Operation> Operations { get; set; } = new();
    }
}