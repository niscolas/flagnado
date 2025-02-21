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
    bool IsOnCooldown = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
        FlagnadoGameplayTags::Player_Cooldown_PickFlag);
    if (IsOnCooldown) {
        UE_LOG(LogTemp, Warning, TEXT("PickFlag is on Cooldown"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AFlagnadoCharacter *Character = Cast<AFlagnadoCharacter>(GetAvatarActorFromActorInfo());
    if (!Character) {
        UE_LOG(LogTemp, Error,
               TEXT("Will not Activate PickFlagGameplayAbility, Owner is not AFlagnadoCharacter"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    USkeletalMeshComponent *SkeletalMeshComponent = Character->GetMesh();

    TriggerEventData->Target->GetRootComponent()->AttachToComponent(
        SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
        PickedFlagSocketName);

    GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(
        FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

    ApplyCooldown(Handle, ActorInfo, ActivationInfo);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
