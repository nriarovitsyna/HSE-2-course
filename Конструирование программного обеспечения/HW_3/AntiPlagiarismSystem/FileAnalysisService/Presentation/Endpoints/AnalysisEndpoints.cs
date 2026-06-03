using FileAnalysisService.UseCases.AnalyzeWork;
using FileAnalysisService.UseCases.GetReport;

namespace FileAnalysisService.Presentation.Endpoints;

public static class AnalysisEndpoints
{
    public static void MapAnalysisEndpoints(this IEndpointRouteBuilder app)
    {
        app.MapPost("/analyze", async (AnalyzeWorkRequest request, IAnalyzeWorkHandler handler) =>
            {
                try
                {
                    var response = await handler.HandleAsync(request);
                    return Results.Created($"/reports/{response.ReportId}", response);
                }
                catch (Exception ex) { return Results.Problem(ex.Message, statusCode: 500); }
            })
            .WithName("AnalyzeWork")
            .WithOpenApi();

        app.MapGet("/reports/{workId:guid}", async (Guid workId, IGetReportHandler handler) =>
            {
                try
                {
                    var response = await handler.HandleAsync(new GetReportRequest(workId));
                    return response.Report is null ? Results.NotFound() : Results.Ok(response.Report);
                }
                catch (Exception ex) { return Results.Problem(ex.Message, statusCode: 500); }
            })
            .WithName("GetReport")
            .WithOpenApi();
    }
}