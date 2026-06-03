namespace HSE_Bank.Models
{
    public class Category
    {
        public int Id { get; set; }
        public OperationType Type { get; set; }
        public string Name { get; set; }

        public Category(int id, OperationType type, string name)
        {
            Id = id;
            Type = type;
            Name = name;
        }
    }
}