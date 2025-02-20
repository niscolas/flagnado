#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "FlagHolderComponent.generated.h"

class UAbilitySystemComponent;
class AFlagnadoFlag;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UFlagHolderComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UFlagHolderComponent();

    UFUNCTION(BlueprintCallable, Category = "Flagnado|Flag Holder")
    void TryPickupFlag(AActor *PossibleFlagActor);

    bool CheckIsHoldingTheFlag() const;

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flag Holder|Debug",
              meta = (AllowPrivateAccess))
    UAbilitySystemComponent *OwnerAbilitySystemComponent;

    UFUNCTION()
    void OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);
};
