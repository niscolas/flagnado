#include "FlagnadoGameMode.h"
#include "Engine/World.h"
#include "Flagnado/Flag/FlagSpawnPoint.h"
#include "Flagnado/FlagnadoFlag.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "FlagnadoPlayerState.h"
#include "HelperMacros.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ReflectedTypeAccessors.h"

AFlagnadoGameMode::AFlagnadoGameMode() : Super() {
}

void AFlagnadoGameMode::BeginPlay() {
    Super::BeginPlay();

    SpawnFlag();
}

void AFlagnadoGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);

    AFlagnadoPlayerState *FlagnadoPlayerState = NewPlayer->GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_LOG_AND_RETURN_IF(!FlagnadoPlayerState, LogTemp, Error,
                               TEXT("Will not proceed with PostLogin logic, "
                                    "invalid PlayerState class"));

    int32 NumDifferentTeams = StaticEnum<ETeam>()->NumEnums() - 1;
    ETeam AssignedTeam = static_cast<ETeam>(TeamCounter % NumDifferentTeams);
    TeamCounter++;

    FlagnadoPlayerState->SetTeam(AssignedTeam);
}

void AFlagnadoGameMode::SpawnFlag() {
    FLAGNADO_LOG_AND_RETURN_IF(!FlagClass, LogTemp, Error, TEXT("FlagClass is invalid"));

    AFlagSpawnPoint *FlagSpawnPoint = Cast<AFlagSpawnPoint>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AFlagSpawnPoint::StaticClass()));
    FLAGNADO_LOG_AND_RETURN_IF(!FlagSpawnPoint, LogTemp, Error,
                               TEXT("Not spawning Flag, no FlagSpawnPoint found"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FlagActor = GetWorld()->SpawnActor<AFlagnadoFlag>(FlagClass, FlagSpawnPoint->GetActorLocation(),
                                                      FRotator::ZeroRotator, SpawnParams);
}
