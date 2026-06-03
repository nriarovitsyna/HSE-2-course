using HSE_Bank.Models;
using HSE_Bank.Repository;
using HSE_Bank.Services.Factories;

namespace HSE_Bank.Services.Facades
{
    public class CategoryFacade : ICategoryFacade
    {
        private readonly IRepository<Category> _repository;
        private readonly ICategoryFactory _factory;

        public CategoryFacade(IRepository<Category> repository, ICategoryFactory factory)
        {
            _repository = repository;
            _factory = factory;
        }

        public Category CreateCategory(OperationType type, string name)
        {
            var category = _factory.CreateCategory(type, name);
            _repository.Add(category);
            return category;
        }

        public Category GetCategory(int id) => _repository.GetById(id);
        public List<Category> GetAllCategories() => _repository.GetAll();

        public List<Category> GetCategoriesByType(OperationType type)
            => _repository.GetAll().Where(c => c.Type == type).ToList();

        public void UpdateCategory(Category category) => _repository.Update(category);
        public void DeleteCategory(int id) => _repository.Delete(id);
    }
}