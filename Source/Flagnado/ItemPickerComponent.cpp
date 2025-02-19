#include "ItemPickerComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "HelperMacros.h"
#include "PickItemGameplayAbility.h"

UItemPickerComponent::UItemPickerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UItemPickerComponent::BeginPlay() {
    Super::BeginPlay();

    IAbilitySystemInterface *OwnerAbilitySystemInterface =
        Cast<IAbilitySystemInterface>(GetOwner());
    if (OwnerAbilitySystemInterface) {
        OwnerAbilitySystemComponent =
            OwnerAbilitySystemInterface->GetAbilitySystemComponent();
    }

    GetOwner()->OnActorBeginOverlap.AddDynamic(
        this, &UItemPickerComponent::OnBeginOverlap);
}

void UItemPickerComponent::OnBeginOverlap(AActor *OverlappedActor,
                                          AActor *OtherActor) {
    FLAGNADO_RETURN_IF(!OwnerAbilitySystemComponent ||
                       !GetOwner()->HasAuthority() || !OtherActor);

    IGameplayTagAssetInterface *OtherActorTagAssetInterface =
        Cast<IGameplayTagAssetInterface>(OtherActor);
    FLAGNADO_RETURN_IF(!OtherActorTagAssetInterface);

    bool IsOtherActorPickable =
        OtherActorTagAssetInterface->HasMatchingGameplayTag(
            FlagnadoGameplayTags::Shared_Status_Pickable);
    FLAGNADO_RETURN_IF(!IsOtherActorPickable);

    FGameplayEventData EventData;
    EventData.Instigator = GetOwner();
    EventData.Target = OtherActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwner(), FlagnadoGameplayTags::Player_Event_Pickup, EventData);
}
