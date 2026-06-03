using Microsoft.EntityFrameworkCore;
using FileAnalysisService.Entities.Models;

namespace FileAnalysisService.Infrastructure.Data;

internal class AppDbContext : DbContext
{
    public DbSet<Report> Reports => Set<Report>();

    public AppDbContext(DbContextOptions<AppDbContext> options)
        : base(options)
    {
    }
    
    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        if (!optionsBuilder.IsConfigured)
        {
            optionsBuilder.UseSqlite("Data Source=analysis.db");
        }
    }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Report>(entity =>
        {
            entity.HasKey(r => r.Id);
            entity.Property(r => r.WorkId).IsRequired();
            entity.Property(r => r.IsPlagiarism).HasDefaultValue(false);
        });
    }
}