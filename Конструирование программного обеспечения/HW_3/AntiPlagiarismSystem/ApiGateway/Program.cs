using Microsoft.OpenApi;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddReverseProxy()
    .LoadFromConfig(builder.Configuration.GetSection("ReverseProxy"));

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v1", new OpenApiInfo
    {
        Title = "AntiPlagiarism System API",
        Version = "v1",
        Description = "API Gateway for AntiPlagiarism System"
    });
});

var app = builder.Build();

app.UseSwagger();
app.UseSwaggerUI(c =>
{
    c.SwaggerEndpoint("/swagger/v1/swagger.json", "API Gateway v1");
    c.RoutePrefix = "swagger";
    c.DocumentTitle = "AntiPlagiarism System";
});

app.MapReverseProxy();

app.MapGet("/health", () => 
    Results.Ok(new { 
        status = "healthy", 
        service = "api-gateway",
        timestamp = DateTime.UtcNow
    }));

app.MapGet("/", () => Results.Redirect("/swagger"));

app.Run();