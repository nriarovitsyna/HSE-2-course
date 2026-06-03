using FileStorageService.UseCases.SubmitWork;
using FileStorageService.Infrastructure.Data;
namespace FileStorageService.Presentation.Endpoints;

public static class WorkEndpoints
{
    public static void MapWorkEndpoints(this WebApplication app)
    {
        app.MapPost("/works", async (HttpContext ctx, ISubmitWorkHandler handler) =>
        {
            var form = await ctx.Request.ReadFormAsync();
            var file = form.Files[0];
            var content = await new StreamReader(file.OpenReadStream()).ReadToEndAsync();
            var request = new SubmitWorkRequest(form["student_id"], form["assignment_id"], file.FileName, content);
            var response = handler.Handle(request);
            return Results.Ok(response);
        });

        app.MapGet("/works/{id}/file", (Guid id, IWorkRepository repo) =>
        {
            var work = repo.GetById(id);
            return work != null ? Results.Text(repo.GetFileContent(work.FilePath)) : Results.NotFound();
        });

        app.MapGet("/works/previous", (string assignmentId, string excludeStudentId, DateTime before, IWorkRepository repo) =>
        {
            var works = repo.GetPreviousForAssignment(assignmentId, excludeStudentId, before);
            return Results.Ok(works.Select(w => new { w.Id, w.StudentId, w.Timestamp, w.Hash }));
        });
    }
}