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
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)

class AFlagnadoCharacter : public ACharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

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

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado|Ability System",
              meta = (AllowPrivateAccess))
    UAbilitySystemComponent *AbilitySystemComponent;

public:
    AFlagnadoCharacter();

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay();

public:
    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputAction *LookAction;

protected:
    void Move(const FInputActionValue &Value);

    void Look(const FInputActionValue &Value);

protected:
    virtual void
    SetupPlayerInputComponent(UInputComponent *InputComponent) override;

public:
    USkeletalMeshComponent *GetMesh1P() const {
        return Mesh1P;
    }

    UCameraComponent *GetFirstPersonCameraComponent() const {
        return FirstPersonCameraComponent;
    }
};
