using FileStorageService.Infrastructure.Data;
using FileStorageService.UseCases.GetPreviousWorks;
using FileStorageService.UseCases.SubmitWork;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContext<WorkDbContext>(options =>
    options.UseSqlite("Data Source=storage.db"));

builder.Services.AddScoped<IWorkRepository, WorkRepository>();
builder.Services.AddScoped<ISubmitWorkHandler, SubmitWorkHandler>();
builder.Services.AddScoped<IGetPreviousWorksHandler, GetPreviousWorksHandler>();

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<WorkDbContext>();
    db.Database.EnsureCreated();
    Directory.CreateDirectory("files");
}

app.UseSwagger();
app.UseSwaggerUI();

app.MapGet("/health", () => 
{
    try
    {
        using var scope = app.Services.CreateScope();
        var db = scope.ServiceProvider.GetRequiredService<WorkDbContext>();
        db.Database.ExecuteSqlRaw("SELECT 1");
        return Results.Ok(new 
        { 
            status = "healthy", 
            service = "storage",
            timestamp = DateTime.UtcNow,
            database = "connected"
        });
    }
    catch (Exception ex)
    {
        return Results.Problem(
            detail: $"Storage service unhealthy: {ex.Message}",
            statusCode: 503
        );
    }
});

app.MapPost("/works", (SubmitWorkRequest request, ISubmitWorkHandler handler) =>
{
    try
    {
        var response = handler.Handle(request);
        return Results.Created($"/works/{response.WorkId}", response);
    }
    catch (Exception ex) { return Results.Problem(ex.Message, statusCode: 500); }
});

app.MapGet("/works/{id:guid}", (Guid id, IWorkRepository repo) =>
{
    try
    {
        var work = repo.GetById(id);
        if (work == null) return Results.NotFound();
        var content = repo.GetFileContent(work.FilePath);
        return Results.Ok(new PreviousWorkDto(work.Id, work.StudentId, work.AssignmentId, Path.GetFileName(work.FilePath), content, work.Timestamp, work.Hash));
    }
    catch (Exception ex) { return Results.Problem(ex.Message, statusCode: 500); }
});

app.MapGet("/works/previous", (
    string assignmentId,
    string excludeStudentId,
    DateTime before,
    IGetPreviousWorksHandler handler) =>
{
    try
    {
        var request = new GetPreviousWorksRequest(assignmentId, excludeStudentId, before);
        return Results.Ok(handler.Handle(request));
    }
    catch (Exception ex) { return Results.Problem(ex.Message, statusCode: 500); }
});

app.MapGet("/", () =>
{
    var html = @"
    <!DOCTYPE html>
    <html>
    <head>
        <title>Storage Service - AntiPlagiarism System</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 40px; }
            h1 { color: #333; }
            ul { list-style-type: none; padding: 0; }
            li { margin: 10px 0; }
            a { color: #007bff; text-decoration: none; }
            a:hover { text-decoration: underline; }
            .container { max-width: 800px; margin: 0 auto; }
            .status { color: green; font-weight: bold; }
        </style>
    </head>
    <body>
        <div class='container'>
            <h1> Storage Service</h1>
            <p><span class='status'> Service is running</span></p>
            <p>API for storing student works in AntiPlagiarism System</p>
            
            <h2> Documentation</h2>
            <ul>
                <li> <a href='/swagger'>Swagger UI</a> - Interactive API documentation</li>
                <li> <a href='/health'>Health Check</a> - Service status</li>
            </ul>
            
            <h2> API Endpoints</h2>
            <ul>
                <li><strong>POST</strong> <code>/works</code> - Submit new work</li>
                <li><strong>GET</strong> <code>/works/{id}</code> - Get work by ID</li>
                <li><strong>GET</strong> <code>/works/previous</code> - Get previous works</li>
            </ul>
            
            <h2> Quick Test</h2>
            <pre>
curl http://localhost:8082/health
curl -X POST http://localhost:8082/works \
  -F ""student_id=test"" \
  -F ""assignment_id=hw1"" \
  -F ""file=@-;filename=work.txt"" <<< ""Work content""
            </pre>
            
            <p><em>Note: This service is part of the AntiPlagiarism System microservices architecture.</em></p>
        </div>
    </body>
    </html>";
    
    return Results.Content(html, "text/html");
})
.WithName("Root")
.WithOpenApi(operation => 
{
    operation.Summary = "Service homepage";
    operation.Description = "Returns HTML page with service information";
    return operation;
});

app.MapGet("/swagger", () => Results.Redirect("/swagger/index.html"));

app.Run("http://0.0.0.0:8080");