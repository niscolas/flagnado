#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include "FlagHolderComponent.generated.h"

class UAbilitySystemComponent;
class AFlagnadoFlag;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsHoldingTheFlagChanged, bool, NewValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UFlagHolderComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UFlagHolderComponent();

    UFUNCTION(BlueprintCallable, Category = "Flagnado|Flag Holder")
    void TryPickupFlag(AActor *PossibleFlagActor);

    UFUNCTION(BlueprintPure, Category = "Flagnado|Flag Holder")
    bool CheckIsHoldingTheFlag() const;

    UFUNCTION(BlueprintCallable, Category = "Flagnado|Flag Holder")
    void DropFlag();

private:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Flag Holder",
              meta = (AllowPrivateAccess))
    FName PickedFlagSocketName;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Flag Holder",
              meta = (AllowPrivateAccess))
    float PickFlagCooldownTimeSeconds = 1.5f;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flag Holder|Debug",
              meta = (AllowPrivateAccess))
    UAbilitySystemComponent *OwnerAbilitySystemComponent;

    UPROPERTY(Replicated,
              VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flag Holder|Debug",
              meta = (AllowPrivateAccess))
    bool IsPickFlagAbilityInCooldown;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flag Holder|Debug",
              meta = (AllowPrivateAccess))
    AFlagnadoFlag *FlagActor;

    UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
    FIsHoldingTheFlagChanged IsHoldingTheFlagChanged;

    UPROPERTY()
    FTimerHandle PickFlagCooldownTimer;

    UFUNCTION()
    void OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);

    UFUNCTION(Server, Reliable)
    void Server_StartPickFlagAbilityCooldown();

    UFUNCTION()
    void ResetPickFlagAbilityCooldown();

    virtual void
    GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
