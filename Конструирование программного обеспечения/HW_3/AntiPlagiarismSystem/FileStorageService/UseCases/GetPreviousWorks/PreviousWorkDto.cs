namespace FileStorageService.UseCases.GetPreviousWorks;

public record PreviousWorkDto(
    Guid WorkId,
    string StudentId,
    string AssignmentId,
    string FileName,
    string Content,
    DateTime Timestamp,
    string? Hash = null);
