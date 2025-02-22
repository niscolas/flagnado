#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameMode.h"
#include "Math/Color.h"
#include "MiscTypes.h"
#include "FlagnadoGameMode.generated.h"

class AFlagSpawnPoint;
class AFlagnadoFlag;

class UMaterialInterface;

UCLASS(minimalapi)

class AFlagnadoGameMode : public AGameMode {
    GENERATED_BODY()

public:
    AFlagnadoGameMode();

    void PostLogin(APlayerController *NewPlayer) override;
    void ResetFlag();

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

    bool HasFetchedExistingTeams;

    virtual void BeginPlay() override;

    void FetchExistingTeams();
    void SpawnFlag();

    UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess))
    AFlagSpawnPoint *GetFlagSpawnPoint() const;
};
