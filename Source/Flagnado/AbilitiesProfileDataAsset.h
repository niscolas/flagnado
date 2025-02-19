#pragma once

#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitiesProfileDataAsset.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()

class FLAGNADO_API UAbilitiesProfileDataAsset : public UDataAsset {
    GENERATED_BODY()

public:
    void GiveAllTo(UAbilitySystemComponent *InAbilitySystemComponent,
                   int8 ApplyLevel = 1);

    void
    GiveCommonAbilitiesTo(UAbilitySystemComponent *InAbilitySystemComponent,
                          TArray<TSubclassOf<UGameplayAbility>> InAbilities,
                          int8 ApplyLevel);

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
    TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
    TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> GameplayTagBasedAbilities;
};
