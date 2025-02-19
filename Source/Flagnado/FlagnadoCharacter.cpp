#include "FlagnadoCharacter.h"
#include "AbilitiesProfileDataAsset.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Flagnado/HelperMacros.h"
#include "FlagnadoProjectile.h"
#include "InputActionValue.h"
#include "ItemPickerComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFlagnadoCharacter::AFlagnadoCharacter() {
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    FirstPersonCameraComponent =
        CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    Mesh1P =
        CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(
        TEXT("AbilitySystemComponent"));

    ItemPickerComponent = CreateDefaultSubobject<UItemPickerComponent>(
        TEXT("ItemPickerComponent"));
}

void AFlagnadoCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AFlagnadoCharacter::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    FLAGNADO_LOG_AND_RETURN_IF(
        AbilitiesProfileDataAsset.IsNull(), LogTemp, Error,
        TEXT("Will not Load Abilities Profile, it's null"));
    UAbilitiesProfileDataAsset *LoadedAbilitiesDataAsset =
        AbilitiesProfileDataAsset.LoadSynchronous();

    FLAGNADO_LOG_AND_RETURN_IF(!LoadedAbilitiesDataAsset, LogTemp, Error,
                               TEXT("Will not Give All Abilities to Character, "
                                    "didn't load Profile properly"));
    LoadedAbilitiesDataAsset->GiveAllTo(AbilitySystemComponent);
}

UAbilitySystemComponent *AFlagnadoCharacter::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}

void AFlagnadoCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
    if (UEnhancedInputComponent *EnhancedInputComponent =
            Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started,
                                           this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed,
                                           this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,
                                           this, &AFlagnadoCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,
                                           this, &AFlagnadoCharacter::Look);
    } else {
        UE_LOG(LogTemplateCharacter, Error,
               TEXT("'%s' Failed to find an Enhanced Input Component! This "
                    "template is built to use the Enhanced Input system. If "
                    "you intend to use the legacy system, then you will need "
                    "to update this C++ file."),
               *GetNameSafe(this));
    }
}

void AFlagnadoCharacter::Move(const FInputActionValue &Value) {
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AFlagnadoCharacter::Look(const FInputActionValue &Value) {
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}
