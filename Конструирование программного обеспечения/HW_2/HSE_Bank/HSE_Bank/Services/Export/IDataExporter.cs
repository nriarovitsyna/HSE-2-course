namespace HSE_Bank.Services.Export
{
    public interface IDataExporter
    {
        ExportResult Export(string filePath, object data);
    }

    public class ExportResult
    {
        public bool Success { get; set; }
        public string Message { get; set; }
        public string FilePath { get; set; }
    }
}