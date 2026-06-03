namespace FileAnalysisService.Entities.Models;

public class Report
{
    public Guid Id { get; private set; } = Guid.NewGuid();
    public Guid WorkId { get; private set; }
    public bool IsPlagiarism { get; private set; }
    public string Details { get; private set; } = string.Empty;
    public string? WordCloudUrl { get; private set; }
    
    public DateTime CreatedAt { get; private set; } = DateTime.UtcNow;

    public Report(Guid workId, bool isPlagiarism, string details, string? wordCloudUrl = null)
    {
        WorkId = workId;
        IsPlagiarism = isPlagiarism;
        Details = details;
        WordCloudUrl = wordCloudUrl;
    }
}