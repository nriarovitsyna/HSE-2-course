using FileStorageService.Entities.Models;

namespace FileStorageService.Infrastructure.Data;

public interface IWorkRepository
{
    void Add(Work work);
    Work? GetById(Guid id);
    string GetFileContent(string path);
    IReadOnlyList<Work> GetPreviousForAssignment(string assignmentId, string excludeStudentId, DateTime before);
}