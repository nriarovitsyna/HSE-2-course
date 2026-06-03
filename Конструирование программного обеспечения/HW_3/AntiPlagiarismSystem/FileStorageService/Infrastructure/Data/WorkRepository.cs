using FileStorageService.Entities.Models;

namespace FileStorageService.Infrastructure.Data;

internal class WorkRepository : IWorkRepository
{
    private readonly WorkDbContext _db;

    public WorkRepository(WorkDbContext db)
    {
        _db = db;
    }

    public void Add(Work work)
    {
        _db.Works.Add(work);
        _db.SaveChanges();
    }

    public Work? GetById(Guid id) => _db.Works.Find(id);

    public string GetFileContent(string path) => File.Exists(path) ? File.ReadAllText(path) : throw new FileNotFoundException();

    public IReadOnlyList<Work> GetPreviousForAssignment(string assignmentId, string excludeStudentId, DateTime before)
    {
        return _db.Works
            .Where(w => w.AssignmentId == assignmentId && w.StudentId != excludeStudentId && w.Timestamp < before)
            .ToList();
    }
}