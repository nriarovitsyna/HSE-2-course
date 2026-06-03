using FileStorageService.Infrastructure.Data;

namespace FileStorageService.UseCases.GetPreviousWorks;

internal class GetPreviousWorksHandler : IGetPreviousWorksHandler
{
    private readonly IWorkRepository _repo;

    public GetPreviousWorksHandler(IWorkRepository repo)
    {
        _repo = repo;
    }

    public GetPreviousWorksResponse Handle(GetPreviousWorksRequest request)
    {
        var works = _repo.GetPreviousForAssignment(
            request.AssignmentId,
            request.ExcludeStudentId,
            request.Before);

        var dtos = works.Select(w => new PreviousWorkDto(
            WorkId: w.Id,
            StudentId: w.StudentId,
            AssignmentId: w.AssignmentId,
            FileName: Path.GetFileName(w.FilePath),
            Content: _repo.GetFileContent(w.FilePath),
            Timestamp: w.Timestamp,
            Hash: w.Hash
        )).ToList();

        return new GetPreviousWorksResponse(dtos);
    }
}