namespace FileStorageService.Entities.Models;

public class Work
{
    public Guid Id { get; private set; } = Guid.NewGuid();
    public string StudentId { get; private set; }
    public string AssignmentId { get; private set; }
    public string FilePath { get; private set; }
    public string? Hash { get; private set; }
    public DateTime Timestamp { get; private set; } = DateTime.UtcNow;
    
    private Work() { }
    
    public Work(string studentId, string assignmentId, string filePath, string content)
    {
        StudentId = studentId;
        AssignmentId = assignmentId;
        FilePath = filePath;
        Hash = ComputeHash(content);
    }

    private static string ComputeHash(string content)
    {
        using var sha256 = System.Security.Cryptography.SHA256.Create();
        var bytes = sha256.ComputeHash(System.Text.Encoding.UTF8.GetBytes(content));
        return Convert.ToBase64String(bytes);
    }
}