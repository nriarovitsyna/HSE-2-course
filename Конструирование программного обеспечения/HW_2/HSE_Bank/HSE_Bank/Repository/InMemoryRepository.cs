namespace HSE_Bank.Repository
{
    public class InMemoryRepository<T> : IRepository<T> where T : class
    {
        private readonly Dictionary<int, T> _entities = new Dictionary<int, T>();
        private readonly Func<T, int> _getId;

        public InMemoryRepository(Func<T, int> getId)
        {
            _getId = getId;
        }

        public void Add(T entity)
        {
            var id = _getId(entity);
            _entities[id] = entity;
        }

        public T GetById(int id) => _entities.ContainsKey(id) ? _entities[id] : null;

        public List<T> GetAll() => _entities.Values.ToList();

        public void Update(T entity)
        {
            var id = _getId(entity);
            if (_entities.ContainsKey(id))
                _entities[id] = entity;
        }

        public void Delete(int id) => _entities.Remove(id);
    }
}