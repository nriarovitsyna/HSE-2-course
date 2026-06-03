using FileAnalysisService.Entities.Models;
using Microsoft.EntityFrameworkCore;

namespace FileAnalysisService.Infrastructure.Data;

internal class ReportRepository : IReportRepository
{
    private readonly AppDbContext _db;

    public ReportRepository(AppDbContext db)
    {
        _db = db;
        _db.Database.EnsureCreated();
    }

    public async Task AddAsync(Report report)
    {
        await _db.Reports.AddAsync(report);
        await _db.SaveChangesAsync();
    }

    public async Task<Report?> GetByWorkIdAsync(Guid workId)
    {
        return await _db.Reports
            .FirstOrDefaultAsync(r => r.WorkId == workId);
    }
}