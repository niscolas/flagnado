#include "FlagnadoGameMode.h"
#include "Engine/World.h"
#include "Flagnado/Flag/FlagSpawnPoint.h"
#include "Flagnado/FlagnadoFlag.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "Flagnado/TeamBase.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "FlagnadoPlayerState.h"
#include "HelperMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogVerbosity.h"
#include "TeamMemberSpawnPoint.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ReflectedTypeAccessors.h"

AFlagnadoGameMode::AFlagnadoGameMode() : Super() {
}

void AFlagnadoGameMode::BeginPlay() {
    Super::BeginPlay();

    FetchExistingTeams();
    SetupGameState();
    SpawnFlag();
}

void AFlagnadoGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);

    AFlagnadoPlayerState *FlagnadoPlayerState = NewPlayer->GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_LOG_AND_RETURN_IF(!FlagnadoPlayerState, LogTemp, Error,
                               TEXT("Invalid PlayerState class"));

    FetchExistingTeams();

    int32 ExistingTeamCount = ExistingTeams.Num();
    FLAGNADO_LOG_AND_RETURN_IF(ExistingTeamCount == 0, LogTemp, Error,
                               TEXT("At least 1 Team Base is necessary"));

    int32 AssignedTeamIndex = CurrentPlayerIndex % ExistingTeamCount;
    ETeam AssignedTeam = ExistingTeams[AssignedTeamIndex];
    CurrentPlayerIndex++;

    FlagnadoPlayerState->SetTeam(AssignedTeam);

    ATeamMemberSpawnPoint *SpawnPoint = GetRandomTeamMemberSpawnPointForTeam(AssignedTeam);
    FLAGNADO_RETURN_IF(!SpawnPoint);

    GetWorld()->GetTimerManager().SetTimerForNextTick([this, SpawnPoint, NewPlayer]() {
        FVector SpawnLocation = SpawnPoint->GetSpawnLocation();
        NewPlayer->GetPawn()->SetActorLocation(SpawnLocation);

        FVector DirectionToFlag = (FlagActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
        FRotator LookAtFlagRotation = DirectionToFlag.Rotation();

        // NewPlayer->SetControlRotation(LookAtFlagRotation);
        // NewPlayer->GetPawn()->SetActorRotation(LookAtFlagRotation);

        PlayersSpawnPoints.Add(NewPlayer, SpawnPoint);
    });
}

void AFlagnadoGameMode::SetupGameState() {
    AFlagnadoGameState *GameState = GetGameState<AFlagnadoGameState>();
    FLAGNADO_LOG_AND_RETURN_IF(!GameState, LogTemp, Error, TEXT("GameState is invalid"));

    GameState->Setup(NumFlagsToWin);
}

void AFlagnadoGameMode::ReloadGame() {
    for (auto CharacterAndSpawnPointPair : PlayersSpawnPoints) {
        CharacterAndSpawnPointPair.Key->GetPawn()->SetActorLocation(
            CharacterAndSpawnPointPair.Value->GetSpawnLocation());

        CharacterAndSpawnPointPair.Key->GetPawn()->SetActorRotation(
            CharacterAndSpawnPointPair.Value->GetActorRotation());
    }
}

void AFlagnadoGameMode::FetchExistingTeams() {
    FLAGNADO_RETURN_IF(HasFetchedExistingTeams);

    TArray<AActor *> TeamBases;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), TeamBases);

    for (AActor *TeamBase : TeamBases) {
        FLAGNADO_CONTINUE_IF(!TeamBase);
        ATeamBase *TeamBaseCasted = Cast<ATeamBase>(TeamBase);
        ExistingTeams.AddUnique(TeamBaseCasted->GetTeam());
    }

    TArray<AActor *> TempTeamMemberSpawnPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamMemberSpawnPoint::StaticClass(),
                                          TempTeamMemberSpawnPoints);

    for (AActor *TempTeamMemberSpawnPoint : TempTeamMemberSpawnPoints) {
        FLAGNADO_CONTINUE_IF(!TempTeamMemberSpawnPoint);

        ATeamMemberSpawnPoint *TeamMemberSpawnPointCasted =
            Cast<ATeamMemberSpawnPoint>(TempTeamMemberSpawnPoint);
        FLAGNADO_LOG_AND_CONTINUE_IF(!TeamMemberSpawnPointCasted, LogTemp, Error,
                                     TEXT("Current TeamMemberSpawnPoint is invalid"));

        ETeam CurrentTeam = TeamMemberSpawnPointCasted->GetTeam();
        if (AllTeamMemberSpawnPointsMap.Contains(CurrentTeam)) {
            AllTeamMemberSpawnPointsMap[CurrentTeam].Content.Add(TeamMemberSpawnPointCasted);
        } else {
            FTeamMemberSpawnPointArray CurrentSpawnPointsArray;
            CurrentSpawnPointsArray.Content.Add(TeamMemberSpawnPointCasted);
            AllTeamMemberSpawnPointsMap.Add(CurrentTeam, CurrentSpawnPointsArray);
        }
    }

    HasFetchedExistingTeams = true;
}

ATeamMemberSpawnPoint *AFlagnadoGameMode::GetRandomTeamMemberSpawnPointForTeam(ETeam InTeam) {
    FTeamMemberSpawnPointArray *CurrentTeamMemberSpawnPoints =
        AllTeamMemberSpawnPointsMap.Find(InTeam);
    FLAGNADO_LOG_AND_RETURN_VALUE_IF(
        !CurrentTeamMemberSpawnPoints || CurrentTeamMemberSpawnPoints->Content.IsEmpty(), nullptr,
        LogTemp, Error, TEXT("Failed to find SpawnPoints for %s"),
        *UEnum::GetValueAsString(InTeam));

    int RandomIndex = FMath::RandRange(0, CurrentTeamMemberSpawnPoints->Content.Num() - 1);
    return CurrentTeamMemberSpawnPoints->Content[RandomIndex];
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
