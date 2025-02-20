#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "PickFlagGameplayAbility.generated.h"

UCLASS()

class FLAGNADO_API UPickFlagGameplayAbility : public UGameplayAbility {
    GENERATED_BODY()

public:
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                         const FGameplayAbilityActorInfo *ActorInfo,
                         const FGameplayAbilityActivationInfo ActivationInfo,
                         const FGameplayEventData *TriggerEventData) override;

private:
    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "PickUp",
              meta = (AllowPrivateAccess))
    FName PickedFlagSocketName;
};
