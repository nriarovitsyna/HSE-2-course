using Microsoft.AspNetCore.Mvc;
using FileAnalysisService.Entities.Models;

namespace FileAnalysisService.Controllers;

[ApiController]
[Route("[controller]")]
public class ReportsController : ControllerBase
{
    private static readonly List<Report> _reports = new();

    [HttpGet]
    public IActionResult GetReports()
    {
        return Ok(_reports);
    }

    [HttpPost]
    public IActionResult AddReport([FromBody] ReportDto dto)
    {
        var report = new Report(
            dto.WorkId,
            dto.IsPlagiarism,
            dto.Details,
            dto.WordCloudUrl
        );

        _reports.Add(report);

        return CreatedAtAction(nameof(GetReports), new { id = report.Id }, report);
    }
}

public record ReportDto(Guid WorkId, bool IsPlagiarism, string Details, string? WordCloudUrl = null);