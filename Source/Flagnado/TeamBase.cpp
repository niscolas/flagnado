#include "TeamBase.h"
#include "Components/SphereComponent.h"
#include "FlagHolderComponent.h"
#include "Flagnado/FlagnadoPlayerState.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "GameFramework/Character.h"
#include "HelperMacros.h"

ATeamBase::ATeamBase() {
    PrimaryActorTick.bCanEverTick = false;

    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollider->SetCollisionObjectType(ECC_WorldDynamic);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore everything by default
    SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereCollider->SetSphereRadius(300.f);
    RootComponent = SphereCollider;
}

void ATeamBase::BeginPlay() {
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void ATeamBase::OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor) {
    FLAGNADO_RETURN_IF(!HasAuthority() || !OtherActor);

    AFlagnadoCharacter *TeamMemberCharacter = Cast<AFlagnadoCharacter>(OtherActor);
    FLAGNADO_RETURN_IF(!TeamMemberCharacter);

    UFlagHolderComponent *FlagHolderComponent =
        OtherActor->GetComponentByClass<UFlagHolderComponent>();
    FLAGNADO_RETURN_IF(!FlagHolderComponent);
    FLAGNADO_RETURN_IF(!FlagHolderComponent->CheckIsHoldingTheFlag());

    AFlagnadoGameState *FlagnadoGameState = GetWorld()->GetGameState<AFlagnadoGameState>();
    FLAGNADO_RETURN_IF(!FlagnadoGameState);

    AFlagnadoPlayerState *FlagnadoPlayerState =
        TeamMemberCharacter->GetPlayerStateChecked<AFlagnadoPlayerState>();
    FLAGNADO_RETURN_IF(!FlagnadoPlayerState);

    FlagnadoGameState->AddTeamOrIncrementTeamScore(FlagnadoPlayerState->GetCurrentTeam());
}
