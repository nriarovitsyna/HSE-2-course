using Microsoft.EntityFrameworkCore;
using FileStorageService.Entities.Models;

namespace FileStorageService.Infrastructure.Data;

internal class WorkDbContext : DbContext
{
    public DbSet<Work> Works { get; set; }

    public WorkDbContext(DbContextOptions<WorkDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Work>(entity =>
        {
            entity.HasKey(w => w.Id);
            entity.Property(w => w.StudentId).IsRequired();
            entity.Property(w => w.AssignmentId).IsRequired();
            entity.Property(w => w.FilePath).IsRequired();
            entity.Property(w => w.Timestamp).IsRequired();
        });
    }
}