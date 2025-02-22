#include "FlagHolderComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Flagnado/FlagnadoCharacter.h"
#include "Flagnado/FlagnadoFlag.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "GameplayTagAssetInterface.h"
#include "HelperMacros.h"
#include "Logging/LogMacros.h"
#include "Net/UnrealNetwork.h"
#include "PickFlagGameplayAbility.h"
#include "TimerManager.h"

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
    UE_LOG(LogTemp, Warning, TEXT("TryPickupFlag (%s) %s"), *PossibleFlagActor->GetName(),
           IsPickFlagAbilityInCooldown ? TEXT("IN COOLDOWN") : TEXT("NOT IN COOLDOWN"));
    FLAGNADO_RETURN_IF(!OwnerAbilitySystemComponent || !PossibleFlagActor ||
                       IsPickFlagAbilityInCooldown);

    AFlagnadoFlag *PossibleFlagActorCasted = Cast<AFlagnadoFlag>(PossibleFlagActor);
    FLAGNADO_RETURN_IF(!PossibleFlagActorCasted);

    FLAGNADO_LOG_AND_RETURN_IF(CheckIsHoldingTheFlag(), LogTemp, Warning,
                               TEXT("Can't PickFlag while already holding it"));

    IGameplayTagAssetInterface *OtherActorTagAssetInterface =
        Cast<IGameplayTagAssetInterface>(PossibleFlagActor);
    FLAGNADO_RETURN_IF(!OtherActorTagAssetInterface);

    bool IsOtherActorPickable = OtherActorTagAssetInterface->HasMatchingGameplayTag(
        FlagnadoGameplayTags::Shared_Status_Pickable);
    FLAGNADO_RETURN_IF(!IsOtherActorPickable);

    FlagActor = PossibleFlagActorCasted;

    FGameplayEventData EventData;
    EventData.Instigator = GetOwner();
    EventData.Target = PossibleFlagActor;

    if (GetOwner()->HasAuthority()) {
        AFlagnadoCharacter *FlagnadoCharacter = Cast<AFlagnadoCharacter>(GetOwner());
        if (!FlagnadoCharacter) {
            UE_LOG(LogTemp, Error, TEXT("Won't PickFlag as a non AFlagnadoCharacter"));
            return;
        }

        USkeletalMeshComponent *SkeletalMeshComponent = FlagnadoCharacter->GetMesh();
        PossibleFlagActor->GetRootComponent()->AttachToComponent(
            SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
            PickedFlagSocketName);

        OwnerAbilitySystemComponent->AddLooseGameplayTag(
            FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

        OnFlagPickedUpSuccessfully();

        Server_StartPickFlagAbilityCooldown();
    }
}

void UFlagHolderComponent::OnFlagPickedUpSuccessfully() {
    FLAGNADO_RETURN_IF(!FlagActor);
    FlagActor->OnPickedUp();
}

void UFlagHolderComponent::Server_StartPickFlagAbilityCooldown_Implementation() {
    UE_LOG(LogTemp, Warning, TEXT("Server_StartPickFlagAbilityCooldown_Implementation"));

    IsPickFlagAbilityInCooldown = true;

    GetOwner()->GetWorldTimerManager().ClearTimer(PickFlagCooldownTimer);
    GetOwner()->GetWorldTimerManager().SetTimer(PickFlagCooldownTimer, this,
                                                &ThisClass::ResetPickFlagAbilityCooldown,
                                                PickFlagCooldownTimeSeconds, false);
}

void UFlagHolderComponent::ResetPickFlagAbilityCooldown() {
    IsPickFlagAbilityInCooldown = false;
}

void UFlagHolderComponent::DropFlag() {
    TArray<AActor *> AttachedActors;
    GetOwner()->GetAttachedActors(AttachedActors);

    for (AActor *AttachedActor : AttachedActors) {
        FLAGNADO_CONTINUE_IF(AttachedActor != FlagActor);

        FlagActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        OwnerAbilitySystemComponent->RemoveLooseGameplayTag(
            FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

        Server_StartPickFlagAbilityCooldown();

        FlagActor->OnDropped();
        FlagActor = nullptr;
    }
}

bool UFlagHolderComponent::CheckIsHoldingTheFlag() const {
    FLAGNADO_RETURN_VALUE_IF(!OwnerAbilitySystemComponent, false);

    bool ReturnValue = OwnerAbilitySystemComponent->HasMatchingGameplayTag(
        FlagnadoGameplayTags::Player_Status_HoldingTheFlag);

    return ReturnValue;
}

void UFlagHolderComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UFlagHolderComponent, IsPickFlagAbilityInCooldown);
}
