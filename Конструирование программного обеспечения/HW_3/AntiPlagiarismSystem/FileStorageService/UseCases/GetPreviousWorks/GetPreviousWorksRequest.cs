namespace FileStorageService.UseCases.GetPreviousWorks;

public record GetPreviousWorksRequest(
    string AssignmentId, 
    string ExcludeStudentId, 
    DateTime Before);