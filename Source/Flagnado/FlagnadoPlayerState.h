#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MiscTypes.h"
#include "FlagnadoPlayerState.generated.h"

UCLASS()

class FLAGNADO_API AFlagnadoPlayerState : public APlayerState {
    GENERATED_BODY()

public:
    AFlagnadoPlayerState();

    ETeam GetCurrentTeam() const;
    void SetTeam(ETeam InTeam);

private:
    UPROPERTY(Replicated,
              VisibleAnywhere,
              BlueprintReadOnly,
              meta = (AllowPrivateAccess))
    ETeam CurrentTeam;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
