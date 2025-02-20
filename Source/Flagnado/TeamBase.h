#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamBase.generated.h"

class USphereComponent;

UCLASS()

class FLAGNADO_API ATeamBase : public AActor {
    GENERATED_BODY()

public:
    ATeamBase();

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    USphereComponent *SphereCollider;

    UFUNCTION(meta = (AllowPrivateAccess))
    void OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);
};
