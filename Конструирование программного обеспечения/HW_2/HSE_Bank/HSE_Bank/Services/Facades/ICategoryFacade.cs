using HSE_Bank.Models;

namespace HSE_Bank.Services.Facades
{
    public interface ICategoryFacade
    {
        Category CreateCategory(OperationType type, string name);
        Category GetCategory(int id);
        List<Category> GetAllCategories();
        List<Category> GetCategoriesByType(OperationType type);
        void UpdateCategory(Category category);
        void DeleteCategory(int id);
    }
}