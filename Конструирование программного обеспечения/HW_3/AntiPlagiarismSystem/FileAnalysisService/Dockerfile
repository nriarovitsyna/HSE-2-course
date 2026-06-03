FROM mcr.microsoft.com/dotnet/aspnet:9.0 AS base
WORKDIR /app
EXPOSE 80

FROM mcr.microsoft.com/dotnet/sdk:9.0 AS build
WORKDIR /src
COPY ["FileAnalysisService.csproj", "./"]
RUN dotnet restore "FileAnalysisService.csproj"
COPY . .
RUN dotnet publish "FileAnalysisService.csproj" -c Release -o /app/publish /p:UseAppHost=false

FROM base AS final
WORKDIR /app
COPY --from=build /app/publish .
ENTRYPOINT ["dotnet", "FileAnalysisService.dll"]