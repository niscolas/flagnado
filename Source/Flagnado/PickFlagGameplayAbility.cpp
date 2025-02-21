#include "PickFlagGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FlagHolderComponent.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoFlag.h"
#include "FlagnadoGameplayTags.h"
#include "HelperMacros.h"

void UPickFlagGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo *ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData *TriggerEventData) {
    FLAGNADO_RETURN_IF(!HasAuthority(&ActivationInfo));

    UE_LOG(LogTemp, Warning, TEXT("Picking %s"), *TriggerEventData->Target->GetName());

    // bool IsOnCooldown = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
    //     FlagnadoGameplayTags::Player_Cooldown_PickFlag);
    // if (IsOnCooldown) {
    //     UE_LOG(LogTemp, Warning, TEXT("PickFlag is on Cooldown"));
    //     EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    //     return;
    // }

    TObjectPtr<const AActor> PossibleFlagActor = TriggerEventData->Target;

    AFlagnadoCharacter *FlagnadoCharacter = Cast<AFlagnadoCharacter>(GetAvatarActorFromActorInfo());
    if (!FlagnadoCharacter) {
        UE_LOG(LogTemp, Error, TEXT("Won't PickFlag as a non AFlagnadoCharacter"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Picking 2 %s"), *PossibleFlagActor->GetName());

    USkeletalMeshComponent *SkeletalMeshComponent = FlagnadoCharacter->GetMesh();
    PossibleFlagActor->GetRootComponent()->AttachToComponent(
        SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
        PickedFlagSocketName);

    GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(
        FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

    if (UFlagHolderComponent *FlagHolderComponent =
            FlagnadoCharacter->FindComponentByClass<UFlagHolderComponent>()) {
        FlagHolderComponent->OnFlagPickedUpSuccessfully();
    }

    UE_LOG(LogTemp, Warning, TEXT("Picking 3 %s"), *TriggerEventData->Target->GetName());

    // ApplyCooldown(Handle, ActorInfo, ActivationInfo);
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
