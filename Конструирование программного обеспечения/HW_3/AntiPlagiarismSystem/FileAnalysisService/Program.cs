using Microsoft.EntityFrameworkCore;
using FileAnalysisService.Infrastructure.Data;
using FileAnalysisService.Presentation.Endpoints;
using FileAnalysisService.UseCases.AnalyzeWork;
using FileAnalysisService.UseCases.GetReport;

var builder = WebApplication.CreateBuilder(args);

builder.Logging.ClearProviders();
builder.Logging.AddConsole();
builder.Logging.SetMinimumLevel(LogLevel.Debug);

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseSqlite("Data Source=analysis.db"));

builder.Services.AddScoped<IReportRepository, ReportRepository>();
builder.Services.AddScoped<IAnalyzeWorkHandler, AnalyzeWorkHandler>();
builder.Services.AddScoped<IGetReportHandler, GetReportHandler>();

builder.Services.AddHttpClient("Storage", client =>
{
    client.BaseAddress = new Uri("http://storage:8080");
});

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

app.Use(async (context, next) =>
{
    Console.WriteLine($"Analysis Request: {context.Request.Method} {context.Request.Path}");
    try
    {
        await next();
        Console.WriteLine($"Analysis Response: {context.Response.StatusCode}");
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Analysis ERROR: {ex.Message}");
        Console.WriteLine($"StackTrace: {ex.StackTrace}");
        throw;
    }
});

using (var scope = app.Services.CreateScope())
{
    try
    {
        var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
        db.Database.EnsureCreated();
        Console.WriteLine("Analysis database initialized");
    }
    catch (Exception ex)
    {
        Console.WriteLine($"Analysis DB ERROR: {ex.Message}");
        throw;
    }
}

app.UseSwagger();
app.UseSwaggerUI();

app.MapGet("/health", () => 
{
    try
    {
        using var scope = app.Services.CreateScope();
        var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
        db.Database.ExecuteSqlRaw("SELECT 1");
        
        var httpClient = new HttpClient();
        var storageResponse = httpClient.GetAsync("http://storage:8080/health").Result;
        var storageStatus = storageResponse.IsSuccessStatusCode ? "connected" : "disconnected";
        
        return Results.Ok(new 
        { 
            status = "healthy", 
            service = "analysis",
            timestamp = DateTime.UtcNow,
            database = "connected",
            storage_service = storageStatus
        });
    }
    catch (Exception ex)
    {
        return Results.Problem(
            detail: $"Analysis service unhealthy: {ex.Message}",
            statusCode: 503
        );
    }
});

app.MapGet("/", () => "Analysis Service is running");

app.MapGroup("/api").MapAnalysisEndpoints();

app.Run("http://0.0.0.0:80");