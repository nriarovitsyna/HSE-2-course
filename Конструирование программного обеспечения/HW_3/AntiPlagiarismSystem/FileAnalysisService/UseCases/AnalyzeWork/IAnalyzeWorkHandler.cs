namespace FileAnalysisService.UseCases.AnalyzeWork;

public interface IAnalyzeWorkHandler
{
    Task<AnalyzeWorkResponse> HandleAsync(AnalyzeWorkRequest request);
}