#include "AbilitiesProfileDataAsset.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "HelperMacros.h"

void UAbilitiesProfileDataAsset::GiveAllTo(
    UAbilitySystemComponent *InAbilitySystemComponent, int8 ApplyLevel) {
    FLAGNADO_RETURN_IF(!InAbilitySystemComponent);

    GiveCommonAbilitiesTo(InAbilitySystemComponent, CommonAbilities,
                          ApplyLevel);
}

void UAbilitiesProfileDataAsset::GiveCommonAbilitiesTo(
    UAbilitySystemComponent *InAbilitySystemComponent,
    TArray<TSubclassOf<UGameplayAbility>> InAbilities,
    int8 ApplyLevel) {
    FLAGNADO_LOG_AND_RETURN_IF(
        !InAbilitySystemComponent || InAbilities.IsEmpty(), LogTemp, Error,
        TEXT("Will not GiveCommonAbilities, invalid inputs"));

    for (TSubclassOf<UGameplayAbility> Ability : InAbilities) {
        FLAGNADO_CONTINUE_IF(!Ability);

        FGameplayAbilitySpec Spec(Ability);
        Spec.SourceObject = InAbilitySystemComponent->GetAvatarActor();
        Spec.Level = ApplyLevel;

        InAbilitySystemComponent->GiveAbility(Spec);
    }
}
