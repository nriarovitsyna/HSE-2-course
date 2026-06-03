using Microsoft.Extensions.DependencyInjection;
using MoscowZoo.Interfaces;
using MoscowZoo.Repository;

namespace MoscowZoo.Services
{
    /// <summary>
    /// Настройка зависимостей приложения через Dependency Injection
    /// </summary>
    public static class DependencyInjection
    {
        /// <summary>
        /// Настройка DI-контейнера для приложения зоопарка
        /// </summary>
        public static ServiceProvider ConfigureServices()
        {
            var services = new ServiceCollection();

            // Регистрируем зависимости как Singleton - один экземпляр на все приложение
            services.AddSingleton<IVeterinaryClinic, VeterinaryClinic>();
            services.AddSingleton<IZooRepository, ZooRepository>();
            services.AddSingleton<IZooService, ZooService>();

            return services.BuildServiceProvider();
        }
    }
}