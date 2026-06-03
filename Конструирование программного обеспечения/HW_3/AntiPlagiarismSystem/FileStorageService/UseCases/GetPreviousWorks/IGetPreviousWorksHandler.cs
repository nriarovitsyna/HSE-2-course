namespace FileStorageService.UseCases.GetPreviousWorks;

public interface IGetPreviousWorksHandler
{
    GetPreviousWorksResponse Handle(GetPreviousWorksRequest request);
}