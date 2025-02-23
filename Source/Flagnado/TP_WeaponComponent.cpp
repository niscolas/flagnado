#include "TP_WeaponComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoHelpers.h"
#include "FlagnadoProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UTP_WeaponComponent::UTP_WeaponComponent() {
    MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire() {
    UE_LOG(LogTemp, Warning, TEXT("(%s) Spawning projectile"),
           *UFlagnadoHelpers::GetNetModeString(GetWorld()));

    if (Character == nullptr || Character->GetController() == nullptr) {
        return;
    }

    if (ProjectileClass != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("(%s) Fire() %s"),
               *UFlagnadoHelpers::GetNetModeString(GetWorld()), *this->GetName());

        UWorld *const World = GetWorld();
        if (World != nullptr) {
            APlayerController *PlayerController =
                Cast<APlayerController>(Character->GetController());
            const FRotator SpawnRotation =
                PlayerController->PlayerCameraManager->GetCameraRotation();
            const FVector SpawnLocation =
                GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.Owner = Character;
            ActorSpawnParams.Instigator = Character;
            ActorSpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

            AActor *ProjectileActor = World->SpawnActor<AFlagnadoProjectile>(
                ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

            if (ProjectileActor) {
                ProjectileActor->SetOwner(Character);
            }
        }
    }

    if (FireSound != nullptr) {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
    }

    if (FireAnimation != nullptr) {
        UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance();
        if (AnimInstance != nullptr) {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

bool UTP_WeaponComponent::AttachWeapon(AFlagnadoCharacter *TargetCharacter) {
    Character = TargetCharacter;

    if (Character == nullptr ||
        Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>()) {
        return false;
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

    Character->AddInstanceComponent(this);

    if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController())) {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent *EnhancedInputComponent =
                Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, Character,
                                               &AFlagnadoCharacter::Shoot);
        }
    }

    return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    if (Character == nullptr) {
        return;
    }

    if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController())) {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer())) {
            Subsystem->RemoveMappingContext(FireMappingContext);
        }
    }
}
