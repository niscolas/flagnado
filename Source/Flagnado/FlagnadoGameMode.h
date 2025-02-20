#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameMode.h"
#include "MiscTypes.h"
#include "FlagnadoGameMode.generated.h"

class AFlagnadoFlag;

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

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Game|Settings|Teams",
              meta = (AllowPrivateAccess))
    TMap<ETeam, UMaterialInterface *> TeamMemberMaterials;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    int32 TeamCounter = 0;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Debug",
              meta = (AllowPrivateAccess))
    AFlagnadoFlag *FlagActor;

    virtual void BeginPlay() override;

    void SpawnFlag();
};
