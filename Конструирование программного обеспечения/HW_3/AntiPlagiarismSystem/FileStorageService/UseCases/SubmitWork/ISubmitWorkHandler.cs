namespace FileStorageService.UseCases.SubmitWork;

public interface ISubmitWorkHandler
{
    SubmitWorkResponse Handle(SubmitWorkRequest request);
}