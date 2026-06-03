using FileAnalysisService.Entities.Models;

namespace FileAnalysisService.Infrastructure.Data;

public interface IReportRepository
{
    Task AddAsync(Report report);
    Task<Report?> GetByWorkIdAsync(Guid workId);
}