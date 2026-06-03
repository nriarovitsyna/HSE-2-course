namespace FileAnalysisService.UseCases.GetReport;

public interface IGetReportHandler
{
    Task<GetReportResponse> HandleAsync(GetReportRequest request);
}