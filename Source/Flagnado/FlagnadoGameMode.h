#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameMode.h"
#include "Math/Color.h"
#include "MiscTypes.h"
#include "FlagnadoGameMode.generated.h"

class AFlagSpawnPoint;
class AFlagnadoFlag;
class ATeamMemberSpawnPoint;
class APlayerController;
class UMaterialInterface;

USTRUCT(BlueprintType)

struct FTeamMemberSpawnPointArray {
    GENERATED_BODY()

public:
    FTeamMemberSpawnPointArray() = default;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<ATeamMemberSpawnPoint *> Content;
};

UCLASS(minimalapi)

class AFlagnadoGameMode : public AGameMode {
    GENERATED_BODY()

public:
    AFlagnadoGameMode();

    void PostLogin(APlayerController *NewPlayer) override;

    UFUNCTION()
    void ResetFlag();

    UFUNCTION()
    void ReloadGame();

    UFUNCTION()
    void SendPlayerToSpawnPoint(AController *PlayerController);

private:
    UPROPERTY(EditDefaultsOnly,
              BlueprintReadWrite,
              Category = "Game|Settings",
              meta = (AllowPrivateAccess))
    int32 NumFlagsToWin = 3;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Game|Settings|Flag",
              meta = (AllowPrivateAccess))
    TSubclassOf<AFlagnadoFlag> FlagClass;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    int32 CurrentPlayerIndex = 0;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    AFlagnadoFlag *FlagActor;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    TArray<ETeam> ExistingTeams;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    TMap<ETeam, FTeamMemberSpawnPointArray> AllTeamMemberSpawnPointsMap;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    TMap<AController *, ATeamMemberSpawnPoint *> PlayersSpawnPoints;

    bool HasFetchedExistingTeams;

    virtual void BeginPlay() override;

    void SetupGameState();
    void FetchExistingTeams();
    void SpawnFlag();
    ATeamMemberSpawnPoint *GetRandomTeamMemberSpawnPointForTeam(ETeam InTeam);

    UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess))
    AFlagSpawnPoint *GetFlagSpawnPoint() const;
};
