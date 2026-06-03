using HSE_Bank.Utilities;
using HSE_Bank.Models;
using HSE_Bank.Repository;
using HSE_Bank.Services.Facades;
using HSE_Bank.Services.Factories;
using HSE_Bank.Services.Import;
using HSE_Bank.Services.Export;
using Microsoft.Extensions.DependencyInjection;

namespace HSE_Bank
{
    class Program
    {
        private static ServiceProvider _serviceProvider;

        static void Main(string[] args)
        {
            Console.Title = "HSE Bank - Учет финансов";
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            ConfigureServices();
            InitializeSampleData();
            RunMainMenu();
        }

        static void ConfigureServices()
        {
            _serviceProvider = new ServiceCollection()
                // Репозитории
                .AddSingleton<IRepository<BankAccount>>(new InMemoryRepository<BankAccount>(a => a.Id))
                .AddSingleton<IRepository<Category>>(new InMemoryRepository<Category>(c => c.Id))
                .AddSingleton<IRepository<Operation>>(new InMemoryRepository<Operation>(o => o.Id))
        
                // Фабрики
                .AddSingleton<IBankAccountFactory, BankAccountFactory>()
                .AddSingleton<ICategoryFactory, CategoryFactory>()
                .AddSingleton<IOperationFactory, OperationFactory>()
        
                // Фасады
                .AddSingleton<IBankAccountFacade, BankAccountFacade>()
                .AddSingleton<ICategoryFacade, CategoryFacade>()
                .AddSingleton<IOperationFacade, OperationFacade>()
                .AddSingleton<IAnalyticsFacade, AnalyticsFacade>()
        
                // Импорт/Экспорт
                .AddSingleton<ImportManager>()
                .AddSingleton<ExportManager>()
        
                // Утилиты
                .AddSingleton<DemoData>()
                .AddSingleton<MenuManager>()
                .BuildServiceProvider();
        }

        static void InitializeSampleData()
        {
            var dataInitializer = _serviceProvider.GetService<DemoData>();
            dataInitializer.InitializeSampleData();
        }

        static void RunMainMenu()
        {
            var menuManager = _serviceProvider.GetService<MenuManager>();
            menuManager.RunMainMenu();
        }
    }
}