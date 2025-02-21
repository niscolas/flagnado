#include "FlagnadoGameMode.h"
#include "Engine/World.h"
#include "Flagnado/Flag/FlagSpawnPoint.h"
#include "Flagnado/FlagnadoFlag.h"
#include "Flagnado/TeamBase.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "FlagnadoPlayerState.h"
#include "HelperMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogVerbosity.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ReflectedTypeAccessors.h"

AFlagnadoGameMode::AFlagnadoGameMode() : Super() {
}

void AFlagnadoGameMode::BeginPlay() {
    Super::BeginPlay();

    if (!HasFetchedExistingTeams) {
        FetchExistingTeams();
    }
    SpawnFlag();
}

void AFlagnadoGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);

    AFlagnadoPlayerState *FlagnadoPlayerState = NewPlayer->GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_LOG_AND_RETURN_IF(!FlagnadoPlayerState, LogTemp, Error,
                               TEXT("Invalid PlayerState class"));

    if (!HasFetchedExistingTeams) {
        FetchExistingTeams();
    }

    int32 ExistingTeamCount = ExistingTeams.Num();
    FLAGNADO_LOG_AND_RETURN_IF(ExistingTeamCount == 0, LogTemp, Error,
                               TEXT("At least 1 Team Base is necessary"));

    int32 AssignedTeamIndex = CurrentPlayerIndex % ExistingTeamCount;
    ETeam AssignedTeam = ExistingTeams[AssignedTeamIndex];
    CurrentPlayerIndex++;

    FlagnadoPlayerState->SetTeam(AssignedTeam);

    AFlagnadoCharacter *FlagnadoCharacter = Cast<AFlagnadoCharacter>(NewPlayer->GetPawn());
    FLAGNADO_LOG_AND_RETURN_IF(!FlagnadoCharacter, LogTemp, Error,
                               TEXT("Invalid Character class for %s"), *NewPlayer->GetName());

    FlagnadoCharacter->OnTeamAssigned(AssignedTeam);
}

void AFlagnadoGameMode::FetchExistingTeams() {
    TArray<AActor *> TeamBases;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), TeamBases);

    for (AActor *TeamBase : TeamBases) {
        FLAGNADO_CONTINUE_IF(!TeamBase);
        ATeamBase *TeamBaseCasted = Cast<ATeamBase>(TeamBase);
        ExistingTeams.AddUnique(TeamBaseCasted->GetTeam());
    }

    HasFetchedExistingTeams = true;
}

void AFlagnadoGameMode::SpawnFlag() {
    FLAGNADO_LOG_AND_RETURN_IF(!FlagClass, LogTemp, Error, TEXT("FlagClass is invalid"));

    AFlagSpawnPoint *FlagSpawnPoint = GetFlagSpawnPoint();
    FLAGNADO_RETURN_IF(!FlagSpawnPoint);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FlagActor = GetWorld()->SpawnActor<AFlagnadoFlag>(FlagClass, FlagSpawnPoint->GetActorLocation(),
                                                      FRotator::ZeroRotator, SpawnParams);
}

void AFlagnadoGameMode::ResetFlag() {
    FLAGNADO_RETURN_IF(!FlagActor);

    AFlagSpawnPoint *FlagSpawnPoint = GetFlagSpawnPoint();
    FLAGNADO_RETURN_IF(!FlagSpawnPoint);

    FlagActor->SetActorLocation(FlagSpawnPoint->GetActorLocation());
}

AFlagSpawnPoint *AFlagnadoGameMode::GetFlagSpawnPoint() const {
    AFlagSpawnPoint *FlagSpawnPoint = Cast<AFlagSpawnPoint>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AFlagSpawnPoint::StaticClass()));
    FLAGNADO_LOG_AND_RETURN_VALUE_IF(!FlagSpawnPoint, nullptr, LogTemp, Error,
                                     TEXT("No FlagSpawnPoint found"));

    return FlagSpawnPoint;
}
