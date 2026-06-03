using FileStorageService.Entities.Models;
using Microsoft.AspNetCore.Mvc;

namespace FileStorageService.Controllers;

[ApiController]
[Route("[controller]")]
public class WorksController : ControllerBase
{
    private static readonly List<Work> _works = new();

    [HttpGet]
    public IActionResult GetWorks()
    {
        return Ok(_works);
    }

    [HttpPost]
    public IActionResult AddWork([FromBody] WorkDto dto)
    {
        var work = new Work(dto.StudentId, dto.AssignmentId, dto.FilePath, dto.Content);
        _works.Add(work);
        return CreatedAtAction(nameof(GetWorks), new { id = work.Id }, work);
    }
}

public record WorkDto(string StudentId, string AssignmentId, string FilePath, string Content);