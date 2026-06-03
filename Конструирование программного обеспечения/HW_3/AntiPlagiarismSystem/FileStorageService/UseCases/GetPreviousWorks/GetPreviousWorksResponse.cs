namespace FileStorageService.UseCases.GetPreviousWorks;

public record GetPreviousWorksResponse(IReadOnlyList<PreviousWorkDto> Works);