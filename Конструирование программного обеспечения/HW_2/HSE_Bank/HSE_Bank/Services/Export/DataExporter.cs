namespace HSE_Bank.Services.Export
{
    public abstract class DataExporter : IDataExporter
    {
        // Шаблонный метод - определяет общий алгоритм экспорта
        public ExportResult Export(string filePath, object data)
        {
            var result = new ExportResult();
            
            try
            {
                var directory = Path.GetDirectoryName(filePath);
                if (!string.IsNullOrEmpty(directory) && !Directory.Exists(directory))
                {
                    Directory.CreateDirectory(directory);
                }

                var content = FormatData(data);
                File.WriteAllText(filePath, content);
                
                result.Success = true;
                result.Message = "Данные успешно экспортированы";
                result.FilePath = filePath;
            }
            catch (Exception ex)
            {
                result.Success = false;
                result.Message = $"Ошибка экспорта: {ex.Message}";
            }

            return result;
        }

        // Абстрактный метод, который будет реализован в конкретных экспортерах
        protected abstract string FormatData(object data);
    }
}