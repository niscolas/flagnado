#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "PickItemGameplayAbility.generated.h"

UCLASS()

class FLAGNADO_API UPickItemGameplayAbility : public UGameplayAbility {
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
    FName PickedItemSocketName;
};
