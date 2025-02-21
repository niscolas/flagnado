#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MiscTypes.h"
#include "FlagnadoGameState.generated.h"

UCLASS()

class FLAGNADO_API AFlagnadoGameState : public AGameState {
    GENERATED_BODY()

public:
    AFlagnadoGameState();

    void AddTeamOrIncrementTeamScore(ETeam InTeam);
    int32 GetTeamScore(ETeam InTeam) const;
    UMaterialInterface *GetMaterialForTeam(ETeam InTeam) const;

private:
    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
    TArray<ETeam> TeamMaterialsKeys;

    UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
    TArray<UMaterialInterface *> TeamMaterialsValues;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    TArray<ETeam> TeamScoresKeys;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    TArray<int32> TeamScoresValues;

    virtual void
    GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
