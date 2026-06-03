namespace HSE_Bank.Services.Import
{
    public abstract class DataImporter : IDataImporter
    {
        // Шаблонный метод - определяет общий алгоритм импорта
        public ImportResult Import(string filePath)
        {
            var result = new ImportResult();
            
            try
            {
                if (!File.Exists(filePath))
                {
                    result.Success = false;
                    result.Message = $"Файл не найден: {filePath}";
                    return result;
                }

                var fileContent = ReadFile(filePath);
                ParseData(fileContent, result);
                
                result.Success = true;
                result.Message = "Данные успешно импортированы";
            }
            catch (Exception ex)
            {
                result.Success = false;
                result.Message = $"Ошибка импорта: {ex.Message}";
            }

            return result;
        }

        protected virtual string ReadFile(string filePath)
        {
            return File.ReadAllText(filePath);
        }

        // Абстрактный метод, который будет реализован в конкретных импортерах
        protected abstract void ParseData(string content, ImportResult result);
    }
}