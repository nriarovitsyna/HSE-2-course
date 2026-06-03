namespace HSE_Bank.Services.Commands
{
    public class CommandDecorator : ICommand
    {
        private readonly ICommand _command;

        public CommandDecorator(ICommand command)
        {
            _command = command;
        }

        public void Execute()
        {
            var startTime = DateTime.Now;
            Console.WriteLine($"Начало выполнения команды: {startTime:HH:mm:ss.fff}");
            
            _command.Execute();
            
            var endTime = DateTime.Now;
            var duration = endTime - startTime;
            Console.WriteLine($"Конец выполнения команды: {endTime:HH:mm:ss.fff}");
            Console.WriteLine($"Время выполнения: {duration.TotalMilliseconds} мс");
        }
    }
}