#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Flagnado/MiscTypes.h"
#include "TeamsColorProfileDataAsset.generated.h"

class UMaterialInterface;

UCLASS()

class FLAGNADO_API UTeamsColorProfileDataAsset : public UDataAsset {
    GENERATED_BODY()

public:
    UTeamsColorProfileDataAsset();

    UFUNCTION(BlueprintPure, Category = "Flagnado|Teams Manager")
    UMaterialInterface *GetMaterialForTeam(ETeam InTeam) const;

    UFUNCTION(BlueprintPure, Category = "Flagnado|Teams Manager")
    FColor GetColorForTeam(ETeam InTeam) const;

private:
    UPROPERTY(EditDefaultsOnly,
              BlueprintReadWrite,
              Category = "Teams Manager",
              meta = (AllowPrivateAccess))
    TMap<ETeam, UMaterialInterface *> TeamsMaterials;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadWrite,
              Category = "Teams Manager",
              meta = (AllowPrivateAccess))
    TMap<ETeam, FColor> TeamsColors;
};
