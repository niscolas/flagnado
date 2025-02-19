#include "FlagnadoFlag.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "Templates/Casts.h"

AFlagnadoFlag::AFlagnadoFlag() {
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = Mesh;

    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

    GameplayTags.AddTag(FlagnadoGameplayTags::Shared_Status_Pickable);
}

void AFlagnadoFlag::BeginPlay() {
    Super::BeginPlay();
}

void AFlagnadoFlag::GetOwnedGameplayTags(
    FGameplayTagContainer &TagContainer) const {
    TagContainer = GameplayTags;
}
