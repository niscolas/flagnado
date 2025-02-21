#include "FlagnadoFlag.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "FlagnadoHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Templates/Casts.h"

AFlagnadoFlag::AFlagnadoFlag() {
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = Mesh;

    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetupAttachment(RootComponent);
    SetCollisionToDefault();

    GameplayTags.AddTag(FlagnadoGameplayTags::Shared_Status_Pickable);
}

void AFlagnadoFlag::BeginPlay() {
    Super::BeginPlay();

    if (HasAuthority()) {
        ShouldEnableCollision = true;
    }

    OnRep_CollisionState();
}

void AFlagnadoFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFlagnadoFlag, ShouldEnableCollision);
}

void AFlagnadoFlag::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const {
    TagContainer = GameplayTags;
}

void AFlagnadoFlag::OnPickedUp() {
    if (HasAuthority()) {
        ShouldEnableCollision = false;
        OnRep_CollisionState();
    }
}

void AFlagnadoFlag::OnDropped() {
    if (HasAuthority()) {
        ShouldEnableCollision = true;
        OnRep_CollisionState();
    }
}

void AFlagnadoFlag::OnRep_CollisionState() {
    FString NetModeStr = UFlagnadoHelpers::GetNetModeString(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_CollisionState called on %s. Collision: %s"),
           *NetModeStr, *GetName(), ShouldEnableCollision ? TEXT("ENABLED") : TEXT("DISABLED"));

    if (ShouldEnableCollision) {
        SetCollisionToDefault();
    } else {
        DisableCollision();
    }
}

void AFlagnadoFlag::SetCollisionToDefault() {
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore everything by default
    SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AFlagnadoFlag::DisableCollision() {
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore everything by default
}
