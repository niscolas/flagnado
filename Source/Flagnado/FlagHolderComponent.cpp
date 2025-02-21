#include "FlagHolderComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Flagnado/FlagnadoFlag.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "HelperMacros.h"
#include "PickFlagGameplayAbility.h"

UFlagHolderComponent::UFlagHolderComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UFlagHolderComponent::BeginPlay() {
    Super::BeginPlay();

    IAbilitySystemInterface *OwnerAbilitySystemInterface =
        Cast<IAbilitySystemInterface>(GetOwner());
    if (OwnerAbilitySystemInterface) {
        OwnerAbilitySystemComponent = OwnerAbilitySystemInterface->GetAbilitySystemComponent();
    }

    GetOwner()->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void UFlagHolderComponent::OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor) {
    TryPickupFlag(OtherActor);
}

void UFlagHolderComponent::TryPickupFlag(AActor *PossibleFlagActor) {
    FLAGNADO_RETURN_IF(!OwnerAbilitySystemComponent || CheckIsHoldingTheFlag() ||
                       !PossibleFlagActor);

    IGameplayTagAssetInterface *OtherActorTagAssetInterface =
        Cast<IGameplayTagAssetInterface>(PossibleFlagActor);
    FLAGNADO_RETURN_IF(!OtherActorTagAssetInterface);

    bool IsOtherActorPickable = OtherActorTagAssetInterface->HasMatchingGameplayTag(
        FlagnadoGameplayTags::Shared_Status_Pickable);
    FLAGNADO_RETURN_IF(!IsOtherActorPickable);

    AFlagnadoFlag *FlagActor = Cast<AFlagnadoFlag>(PossibleFlagActor);
    FLAGNADO_RETURN_IF(!FlagActor);

    UE_LOG(LogTemp, Warning, TEXT("Picking %s"), *PossibleFlagActor->GetName());

    FGameplayEventData EventData;
    EventData.Instigator = GetOwner();
    EventData.Target = FlagActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwner(), FlagnadoGameplayTags::Player_Event_FlagPickup, EventData);
}

void UFlagHolderComponent::DropFlag() {
    TArray<AActor *> AttachedActors;
    GetOwner()->GetAttachedActors(AttachedActors);

    for (AActor *AttachedActor : AttachedActors) {
        AFlagnadoFlag *PossibleFlagActor = Cast<AFlagnadoFlag>(AttachedActor);

        UE_LOG(LogTemp, Warning, TEXT("Trying to drop %s"), *PossibleFlagActor->GetName());

        if (!PossibleFlagActor) {
            UE_LOG(LogTemp, Warning, TEXT("Not dropping %s, it's not a Flag"),
                   *PossibleFlagActor->GetName());
            continue;
        }

        UE_LOG(LogTemp, Warning, TEXT("Dropping %s"), *PossibleFlagActor->GetName());

        UE_LOG(LogTemp, Warning, TEXT("Is (%s) attached to (%s)? %s"),
               *PossibleFlagActor->GetName(), *GetOwner()->GetActorNameOrLabel(),
               PossibleFlagActor->IsAttachedTo(GetOwner()) ? TEXT("Yes") : TEXT("No"));

        PossibleFlagActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        OwnerAbilitySystemComponent->RemoveLooseGameplayTag(
            FlagnadoGameplayTags::Player_Status_HoldingTheFlag);
    }
}

bool UFlagHolderComponent::CheckIsHoldingTheFlag() const {
    FLAGNADO_RETURN_VALUE_IF(!OwnerAbilitySystemComponent, false);

    bool ReturnValue = OwnerAbilitySystemComponent->HasMatchingGameplayTag(
        FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

    return ReturnValue;
}
