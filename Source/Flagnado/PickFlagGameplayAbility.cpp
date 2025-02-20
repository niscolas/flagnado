#include "PickFlagGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FlagHolderComponent.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameplayTags.h"
#include "HelperMacros.h"

void UPickFlagGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo *ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData *TriggerEventData) {
    AFlagnadoCharacter *Character = Cast<AFlagnadoCharacter>(GetAvatarActorFromActorInfo());
    FLAGNADO_LOG_AND_RETURN_IF(!Character, LogTemp, Error,
                               TEXT("Will not Activate PickFlagGameplayAbility, Owner is not "
                                    "AFlagnadoCharacter"));

    USkeletalMeshComponent *SkeletalMeshComponent = Character->GetMesh();

    TriggerEventData->Target->GetRootComponent()->AttachToComponent(
        SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
        PickedFlagSocketName);

    GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(
        FlagnadoGameplayTags::Player_Status_HoldingTheFlag);
}
