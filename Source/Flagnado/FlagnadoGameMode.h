#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameMode.h"
#include "MiscTypes.h"
#include "FlagnadoGameMode.generated.h"

UCLASS(minimalapi)

class AFlagnadoGameMode : public AGameMode {
    GENERATED_BODY()

public:
    AFlagnadoGameMode();

    void PostLogin(APlayerController *NewPlayer) override;

private:
    UPROPERTY(EditDefaultsOnly,
              BlueprintReadWrite,
              Category = "Game|Settings",
              meta = (AllowPrivateAccess))
    int32 NumFlagsToWin = 3;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Game|Team",
              meta = (AllowPrivateAccess))
    TMap<ETeam, UMaterialInterface *> TeamMemberMaterials;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Game|Team|Debug",
              meta = (AllowPrivateAccess))
    int32 TeamCounter = 0;
};
