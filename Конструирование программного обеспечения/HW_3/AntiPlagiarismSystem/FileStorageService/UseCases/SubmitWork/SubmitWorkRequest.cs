namespace FileStorageService.UseCases.SubmitWork;

public record SubmitWorkRequest(string StudentId, string AssignmentId, string FileName, string Content);