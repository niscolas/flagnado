#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FlagnadoCharacter.generated.h"

class UInputComponent;
class UAbilitySystemComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UItemPickerComponent;
class UAbilitiesProfileDataAsset;
struct FInputActionValue;

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

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "",
              meta = (AllowPrivateAccess))
    UItemPickerComponent *ItemPickerComponent;

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

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Ability System",
              meta = (AllowPrivateAccess))
    TSoftObjectPtr<UAbilitiesProfileDataAsset> AbilitiesProfileDataAsset;

    virtual void BeginPlay();
    void PossessedBy(AController *NewController) override;

    void Move(const FInputActionValue &Value);

    void Look(const FInputActionValue &Value);

    virtual void
    SetupPlayerInputComponent(UInputComponent *InputComponent) override;
};
