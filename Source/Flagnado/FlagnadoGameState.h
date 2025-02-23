#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MiscTypes.h"
#include "UObject/ObjectMacros.h"
#include "FlagnadoGameState.generated.h"

class UTeamsColorProfileDataAsset;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnyTeamDataChanged);

UCLASS()

class FLAGNADO_API AFlagnadoGameState : public AGameState {
    GENERATED_BODY()

public:
    AFlagnadoGameState();

    UFUNCTION(BlueprintPure, Category = "Flagnado|Game State")
    TArray<UTeamData *> GetTeamsData() const;

    void Setup(int32 InNumFlagsToWin, TArray<ETeam> ExistingTeams);

    void AddTeamOrIncrementTeamScore(ETeam InTeam);
    int32 GetTeamScore(ETeam InTeam) const;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
    UTeamsColorProfileDataAsset *TeamsColorProfileDataAsset;

    UPROPERTY(Replicated,
              VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado Game State|Debug",
              meta = (AllowPrivateAccess))
    int32 NumFlagsToWin;

    UPROPERTY(Replicated,
              VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado Game State|Debug",
              meta = (AllowPrivateAccess))
    TArray<ETeam> TeamScoresKeys;

    UPROPERTY(ReplicatedUsing = OnRep_TeamScoresValues,
              VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado Game State|Debug",
              meta = (AllowPrivateAccess))
    TArray<int32> TeamScoresValues;

    UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
    FAnyTeamDataChanged AnyTeamDataChanged;

    UFUNCTION()
    void OnRep_TeamScoresValues();

    void SetupInitialTeamsScores(TArray<ETeam> ExistingTeams);
    void BroadcastAnyTeamDataChanged();

    virtual void
    GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
