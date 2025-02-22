#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "Flagnado/MiscTypes.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FlagnadoCharacter.generated.h"

class UInputComponent;
class UAbilitySystemComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UFlagHolderComponent;
class UAbilitiesProfileDataAsset;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDied);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)

class AFlagnadoCharacter : public ACharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

public:
    AFlagnadoCharacter();

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    USkeletalMeshComponent *GetMesh1P() const {
        return Mesh1P;
    }

    UCameraComponent *GetFirstPersonCameraComponent() const {
        return FirstPersonCameraComponent;
    }

    bool TryGetAssignedTeam(ETeam &OutTeam) const;

private:
    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Mesh,
              meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *Mesh1P;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Camera,
              meta = (AllowPrivateAccess = "true"))
    UCameraComponent *FirstPersonCameraComponent;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Ability System",
              meta = (AllowPrivateAccess))
    UAbilitySystemComponent *AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "", meta = (AllowPrivateAccess))
    UFlagHolderComponent *FlagHolderComponent;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    UInputAction *JumpAction;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    UInputAction *MoveAction;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputAction *LookAction;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    UInputAction *TestAction;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Ability System",
              meta = (AllowPrivateAccess))
    TSoftObjectPtr<UAbilitiesProfileDataAsset> AbilitiesProfileDataAsset;

    UPROPERTY(BlueprintCallable,
              BlueprintAssignable,
              Category = "State",
              meta = (AllowPrivateAccess))
    FDied Died;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado Character|Debug",
              meta = (AllowPrivateAccess))
    bool HasUpdatedMeshesProperly;

    virtual void BeginPlay();
    virtual void PossessedBy(AController *NewController) override;
    virtual void OnRep_PlayerState() override;
    virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;

    UFUNCTION(Server, Reliable)
    void Server_UpdateMeshesColorsOnce();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_UpdateMeshesColorsOnce();

    UFUNCTION()
    void HandleUpdateMeshesColors();

    void SetupAbilitySystemComponent();
    void Test(const FInputActionValue &Value);
    void Move(const FInputActionValue &Value);
    void Look(const FInputActionValue &Value);

    UPROPERTY()
    FTimerHandle TimerHandle;
};
