#include "ItemPickerComponent.h"
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
    FLAGNADO_RETURN_IF(!OwnerAbilitySystemComponent || !OtherActor);

    IGameplayTagAssetInterface *OtherActorTagAssetInterface =
        Cast<IGameplayTagAssetInterface>(OtherActor);
    FLAGNADO_RETURN_IF(!OtherActorTagAssetInterface);

    bool OtherActorIsPickable =
        OtherActorTagAssetInterface->HasMatchingGameplayTag(
            FlagnadoGameplayTags::Shared_Status_Pickable);
    FLAGNADO_RETURN_IF(!OtherActorIsPickable);

    OwnerAbilitySystemComponent->TryActivateAbilityByClass(
        UPickItemGameplayAbility::StaticClass());
}
