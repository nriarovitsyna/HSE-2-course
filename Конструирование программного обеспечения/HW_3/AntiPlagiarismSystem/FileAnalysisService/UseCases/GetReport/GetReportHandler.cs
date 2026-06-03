using FileAnalysisService.Infrastructure.Data;

namespace FileAnalysisService.UseCases.GetReport;

internal class GetReportHandler : IGetReportHandler
{
    private readonly IReportRepository _repo;

    public GetReportHandler(IReportRepository repo)
    {
        _repo = repo;
    }

    public async Task<GetReportResponse> HandleAsync(GetReportRequest request)
    {
        var report = await _repo.GetByWorkIdAsync(request.WorkId);
        return new GetReportResponse(report);
    }
}