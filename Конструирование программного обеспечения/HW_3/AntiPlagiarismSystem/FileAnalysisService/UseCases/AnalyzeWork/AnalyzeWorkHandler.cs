using FileAnalysisService.Entities.Models;
using FileAnalysisService.Infrastructure.Data;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;

namespace FileAnalysisService.UseCases.AnalyzeWork;

internal class AnalyzeWorkHandler : IAnalyzeWorkHandler
{
    private readonly IReportRepository _repo;
    private readonly HttpClient _httpClient;
    
    private record StorageWorkDto(
        Guid WorkId,
        string StudentId,
        string AssignmentId,
        string FileName,
        string Content,
        DateTime Timestamp,
        string? Hash = null);

    public AnalyzeWorkHandler(IReportRepository repo, IHttpClientFactory factory)
    {
        _repo = repo;
        _httpClient = factory.CreateClient("Storage");
    }

    public async Task<AnalyzeWorkResponse> HandleAsync(AnalyzeWorkRequest request)
    {
        var storageWork = await _httpClient.GetFromJsonAsync<StorageWorkDto>($"/works/{request.WorkId}");
        if (storageWork == null) 
            throw new InvalidOperationException($"Work {request.WorkId} not found in Storage Service");
        
        var analysisWork = new PreviousWorkDto(
            storageWork.WorkId,
            storageWork.StudentId,
            storageWork.AssignmentId,
            storageWork.FileName,
            storageWork.Content,
            storageWork.Timestamp,
            storageWork.Hash
        );

        var content = analysisWork.Content;
        
        var prevUrl = $"/works/previous?" +
                     $"assignmentId={analysisWork.AssignmentId}" +
                     $"&excludeStudentId={analysisWork.StudentId}" +
                     $"&before={analysisWork.Timestamp:o}";

        var response = await _httpClient.GetAsync(prevUrl);
        response.EnsureSuccessStatusCode();
        
        var jsonString = await response.Content.ReadAsStringAsync();
        Console.WriteLine($"Received from storage: {jsonString}");
        var previousWorks = ParsePreviousWorks(jsonString);
        
        var currentHash = ComputeHash(content);
        var match = previousWorks.FirstOrDefault(p => p.Hash == currentHash);

        bool isPlagiarism = match != null;
        string details = isPlagiarism
            ? $"Полное совпадение с работой студента {match!.StudentId} от {match.Timestamp:dd.MM.yyyy HH:mm}"
            : "Плагиат не обнаружен (по точному совпадению хеша)";
        
        var safeText = content.Length > 4000 ? content[..4000] : content;
        var encoded = Uri.EscapeDataString(safeText);
        var wordCloudUrl = $"https://quickchart.io/wordcloud?text={encoded}&width=800&height=400&format=png&backgroundColor=white&fontFamily=arial";
        
        var report = new Report(request.WorkId, isPlagiarism, details, wordCloudUrl);
        await _repo.AddAsync(report);

        return new AnalyzeWorkResponse(report.Id);
    }

    private static List<PreviousWorkDto> ParsePreviousWorks(string jsonString)
    {
        try
        {
            var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
            
            using var doc = JsonDocument.Parse(jsonString);
            var root = doc.RootElement;
            
            List<PreviousWorkDto> result = new();
            
            if (root.ValueKind == JsonValueKind.Array)
            {
                foreach (var item in root.EnumerateArray())
                {
                    result.Add(ParseWorkItem(item));
                }
            }
            else if (root.ValueKind == JsonValueKind.Object && 
                     root.TryGetProperty("works", out var worksElement) &&
                     worksElement.ValueKind == JsonValueKind.Array)
            {
                foreach (var item in worksElement.EnumerateArray())
                {
                    result.Add(ParseWorkItem(item));
                }
            }
            else if (root.ValueKind == JsonValueKind.Object && 
                     root.TryGetProperty("workId", out _))
            {
                result.Add(ParseWorkItem(root));
            }
            
            Console.WriteLine($"Parsed {result.Count} previous works");
            return result;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error parsing previous works: {ex.Message}");
            Console.WriteLine($"JSON: {jsonString}");
            return new List<PreviousWorkDto>();
        }
    }

    private static PreviousWorkDto ParseWorkItem(JsonElement item)
    {
        return new PreviousWorkDto(
            item.GetProperty("workId").GetGuid(),
            item.GetProperty("studentId").GetString() ?? "",
            item.GetProperty("assignmentId").GetString() ?? "",
            item.GetProperty("fileName").GetString() ?? "",
            item.GetProperty("content").GetString() ?? "",
            item.GetProperty("timestamp").GetDateTime(),
            item.TryGetProperty("hash", out var hashProp) ? hashProp.GetString() : null
        );
    }

    private static string ComputeHash(string input)
    {
        using var sha256 = SHA256.Create();
        var bytes = sha256.ComputeHash(Encoding.UTF8.GetBytes(input));
        return Convert.ToBase64String(bytes);
    }
}