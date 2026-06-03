using FileStorageService.Entities.Models;
using FileStorageService.Infrastructure.Data;

namespace FileStorageService.UseCases.SubmitWork;

internal class SubmitWorkHandler : ISubmitWorkHandler
{
    private readonly IWorkRepository _repo;

    public SubmitWorkHandler(IWorkRepository repo)
    {
        _repo = repo;
    }

    public SubmitWorkResponse Handle(SubmitWorkRequest request)
    {
        var filePath = Path.Combine("files", $"{Guid.NewGuid()}_{request.FileName}");
        Directory.CreateDirectory("files");
        File.WriteAllText(filePath, request.Content);

        var work = new Work(request.StudentId, request.AssignmentId, filePath, request.Content);
        _repo.Add(work);

        return new SubmitWorkResponse(work.Id);
    }
}