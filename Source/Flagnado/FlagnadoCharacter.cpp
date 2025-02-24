#include "FlagnadoCharacter.h"
#include "AbilitiesProfileDataAsset.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FlagHolderComponent.h"
#include "Flagnado/FlagnadoGameMode.h"
#include "Flagnado/FlagnadoGameState.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "Flagnado/FlagnadoPlayerState.h"
#include "Flagnado/HelperMacros.h"
#include "Flagnado/MiscTypes.h"
#include "FlagnadoGameplayTags.h"
#include "FlagnadoProjectile.h"
#include "InputActionValue.h"
#include "Logging/LogMacros.h"
#include "TP_WeaponComponent.h"
#include "TeamsColorProfileDataAsset.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFlagnadoCharacter::AFlagnadoCharacter() {
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    FirstPersonCameraComponent =
        CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetOwnerNoSee(false);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

    GetMesh()->SetOnlyOwnerSee(false);
    GetMesh()->SetOwnerNoSee(true);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    AbilitySystemComponent =
        CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    FlagHolderComponent = CreateDefaultSubobject<UFlagHolderComponent>(TEXT("FlagHolderComponent"));

    TeamEnumToTagMap.Add(ETeam::Red, FlagnadoGameplayTags::Player_Team_Red);
    TeamEnumToTagMap.Add(ETeam::Blue, FlagnadoGameplayTags::Player_Team_Blue);
    TeamEnumToTagMap.Add(ETeam::Green, FlagnadoGameplayTags::Player_Team_Green);
    TeamEnumToTagMap.Add(ETeam::Yellow, FlagnadoGameplayTags::Player_Team_Yellow);
    TeamEnumToTagMap.Add(ETeam::Purple, FlagnadoGameplayTags::Player_Team_Purple);
}

void AFlagnadoCharacter::BeginPlay() {
    Super::BeginPlay();
    SpawnAndAttachWeapon();
}

void AFlagnadoCharacter::PossessedBy(AController *NewController) {
    Super::PossessedBy(NewController);

    SetupAbilitySystemComponent();
    GetWorldTimerManager().SetTimer(UpdateMeshesColorsTimerHandle, this,
                                    &ThisClass::Multicast_UpdateMeshesColorsOnce, 1.f, false);
    GetWorldTimerManager().SetTimer(AddTeamTagTimerHandle, this, &ThisClass::AddTeamTag, 1.f,
                                    false);
}

void AFlagnadoCharacter::OnRep_PlayerState() {
    Super::OnRep_PlayerState();

    PlayerStateUpdated.Broadcast();
    GetWorldTimerManager().SetTimer(UpdateMeshesColorsTimerHandle, this,
                                    &ThisClass::Multicast_UpdateMeshesColorsOnce, 1.f, false);
}

UAbilitySystemComponent *AFlagnadoCharacter::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}

void AFlagnadoCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    if (UEnhancedInputComponent *EnhancedInputComponent =
            Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
                                           &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
                                           &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                                           &AFlagnadoCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
                                           &AFlagnadoCharacter::Look);
    } else {
        UE_LOG(LogTemplateCharacter, Error,
               TEXT("'%s' Failed to find an Enhanced Input Component! This "
                    "template is built to use the Enhanced Input system. If "
                    "you intend to use the legacy system, then you will need "
                    "to update this C++ file."),
               *GetNameSafe(this));
    }
}

void AFlagnadoCharacter::SetupAbilitySystemComponent() {
    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    FLAGNADO_LOG_AND_RETURN_IF(AbilitiesProfileDataAsset.IsNull(), LogTemp, Error,
                               TEXT("Will not Load Abilities Profile, it's null"));
    UAbilitiesProfileDataAsset *LoadedAbilitiesDataAsset =
        AbilitiesProfileDataAsset.LoadSynchronous();

    FLAGNADO_LOG_AND_RETURN_IF(!LoadedAbilitiesDataAsset, LogTemp, Error,
                               TEXT("Will not Give All Abilities to Character, "
                                    "didn't load Profile properly"));
    LoadedAbilitiesDataAsset->GiveAllTo(AbilitySystemComponent);
}

void AFlagnadoCharacter::AddTeamTag() {
    FLAGNADO_RETURN_IF(!HasAuthority());

    AFlagnadoPlayerState *PlayerState = GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_RETURN_IF(!PlayerState);

    FGameplayTag *TeamTag = TeamEnumToTagMap.Find(PlayerState->GetCurrentTeam());
    FLAGNADO_RETURN_IF(!TeamTag);

    AbilitySystemComponent->AddLooseGameplayTag(*TeamTag);
}

void AFlagnadoCharacter::SpawnAndAttachWeapon() {
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = this;
    SpawnParameters.Instigator = this;
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor *WeaponActor = GetWorld()->SpawnActor<AActor>(WeaponClass, FVector::ZeroVector,
                                                         FRotator::ZeroRotator, SpawnParameters);
    FLAGNADO_LOG_AND_RETURN_IF(!WeaponActor, LogTemp, Error, TEXT("Failed to spawn weapon"));

    WeaponActor->SetOwner(this);

    WeaponComponent = WeaponActor->GetComponentByClass<UTP_WeaponComponent>();
    FLAGNADO_LOG_AND_RETURN_IF(!WeaponComponent, LogTemp, Error,
                               TEXT("WeaponActor doesn't have a UTP_WeaponComponent"));

    WeaponComponent->AttachWeapon(this);
}

void AFlagnadoCharacter::OnShootInput() {
    Server_Shoot();
    WeaponComponent->PlayFireFX();
}

void AFlagnadoCharacter::Server_Shoot_Implementation() {
    FLAGNADO_LOG_AND_RETURN_IF(!WeaponComponent, LogTemp, Error,
                               TEXT("WeaponComponent doesn't exist"));

    WeaponComponent->Fire();
}

bool AFlagnadoCharacter::TryGetAssignedTeam(ETeam &OutTeam) const {
    AFlagnadoPlayerState *FlagnadoPlayerState = GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_LOG_AND_RETURN_VALUE_IF(!FlagnadoPlayerState, false, LogTemp, Error,
                                     TEXT("Can't get AssignedTeam, Invalid FlagnadoPlayerState"));

    OutTeam = FlagnadoPlayerState->GetCurrentTeam();
    return true;
}

void AFlagnadoCharacter::Multicast_UpdateMeshesColorsOnce_Implementation() {
    HandleUpdateMeshesColorsOnce();
}

void AFlagnadoCharacter::HandleUpdateMeshesColorsOnce() {
    FLAGNADO_RETURN_IF(HasUpdatedMeshesProperly);
    FLAGNADO_LOG_AND_RETURN_IF(!TeamsColorProfileDataAsset, LogTemp, Error,
                               TEXT("(%s) Invalid TeamsColorProfileDataAsset"),
                               *GetActorNameOrLabel());

    ETeam CurrentTeam;
    FLAGNADO_LOG_AND_RETURN_IF(!TryGetAssignedTeam(CurrentTeam), LogTemp, Error,
                               TEXT("(%s) No team assigned"), *GetActorNameOrLabel());

    UMaterialInterface *TeamMaterial = TeamsColorProfileDataAsset->GetMaterialForTeam(CurrentTeam);
    FLAGNADO_LOG_AND_RETURN_IF(!TeamMaterial, LogTemp, Error,
                               TEXT("(%s) Couldn't find Team Material"), *GetActorNameOrLabel());

    GetMesh()->SetMaterial(0, TeamMaterial);
    GetMesh1P()->SetMaterial(0, TeamMaterial);

    HasUpdatedMeshesProperly = true;
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

void AFlagnadoCharacter::OnShot() {
    if (HasAuthority()) {
        HandleOnShot();
    } else {
        Server_OnShot();
    }
}

void AFlagnadoCharacter::Server_OnShot_Implementation() {
    HandleOnShot();
}

void AFlagnadoCharacter::HandleOnShot() {
    FlagHolderComponent->DropFlag();

    AFlagnadoGameMode *FlagnadoGameMode = GetWorld()->GetAuthGameMode<AFlagnadoGameMode>();
    FLAGNADO_RETURN_IF(!FlagnadoGameMode);

    FlagnadoGameMode->SendPlayerToSpawnPoint(Controller);
}
