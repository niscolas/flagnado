#include "PickItemGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Flagnado/FlagnadoCharacter.h"
#include "HelperMacros.h"

void UPickItemGameplayAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData) {
    AFlagnadoCharacter *Character =
        Cast<AFlagnadoCharacter>(GetAvatarActorFromActorInfo());
    FLAGNADO_RETURN_IF(!Character);

    USkeletalMeshComponent *SkeletalMeshComponent = Character->GetMesh1P();

    TriggerEventData->Target->GetRootComponent()->AttachToComponent(
        SkeletalMeshComponent,
        FAttachmentTransformRules::SnapToTargetIncludingScale,
        PickedItemSocketName);
}
