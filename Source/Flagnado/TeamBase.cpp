#include "TeamBase.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "FlagHolderComponent.h"
#include "Flagnado/FlagnadoGameMode.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "Flagnado/FlagnadoPlayerState.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "GameFramework/Character.h"
#include "HelperMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/Casts.h"

ATeamBase::ATeamBase() {
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = Mesh;

    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollider->SetCollisionObjectType(ECC_WorldDynamic);
    SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignore everything by default
    SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereCollider->SetSphereRadius(300.f);
}

void ATeamBase::BeginPlay() {
    Super::BeginPlay();

    UpdateMeshColor();
    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void ATeamBase::UpdateMeshColor() {
    AFlagnadoGameState *FlagnadoGameState = GetWorld()->GetGameState<AFlagnadoGameState>();
    FLAGNADO_RETURN_IF(!FlagnadoGameState);

    UMaterialInterface *TeamMaterial = FlagnadoGameState->GetMaterialForTeam(Team);
    FLAGNADO_RETURN_IF(!TeamMaterial);

    Mesh->SetMaterial(0, TeamMaterial);
}

void ATeamBase::OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor) {
    IncrementTeamScore(OtherActor);
}

ETeam ATeamBase::GetTeam() const {
    return Team;
}

void ATeamBase::IncrementTeamScore(AActor *PossibleTeamMember) {
    FLAGNADO_RETURN_IF(!PossibleTeamMember);

    UE_LOG(LogTemp, Warning, TEXT("HEREE 4"));

    AFlagnadoCharacter *TeamMemberCharacter = Cast<AFlagnadoCharacter>(PossibleTeamMember);
    FLAGNADO_RETURN_IF(!TeamMemberCharacter);

    UFlagHolderComponent *TeamMemberFlagHolderComponent =
        PossibleTeamMember->GetComponentByClass<UFlagHolderComponent>();
    FLAGNADO_RETURN_IF(!TeamMemberFlagHolderComponent);
    FLAGNADO_RETURN_IF(!TeamMemberFlagHolderComponent->CheckIsHoldingTheFlag());

    AFlagnadoPlayerState *TeamMemberPlayerState =
        TeamMemberCharacter->GetPlayerStateChecked<AFlagnadoPlayerState>();
    FLAGNADO_RETURN_IF(!TeamMemberPlayerState);

    UE_LOG(LogTemp, Warning, TEXT("HEREE 1"));

    ETeam MemberTeam = TeamMemberPlayerState->GetCurrentTeam();
    FLAGNADO_RETURN_IF(MemberTeam != Team);

    UE_LOG(LogTemp, Warning, TEXT("HEREE 2"));

    if (HasAuthority()) {
        HandleIncrementTeamScore();
    } else {
        Server_IncrementTeamScore();
    }

    TeamMemberFlagHolderComponent->DropFlag();

    AFlagnadoGameMode *FlagnadoGameMode =
        UFlagnadoHelpers::GetGameMode<AFlagnadoGameMode>(GetWorld());
    FLAGNADO_RETURN_IF(!FlagnadoGameMode);

    UE_LOG(LogTemp, Warning, TEXT("HEREE 3"));

    FlagnadoGameMode->ResetFlag();
}

void ATeamBase::Server_IncrementTeamScore_Implementation() {
    HandleIncrementTeamScore();
}

bool ATeamBase::Server_IncrementTeamScore_Validate() {
    return true;
}

void ATeamBase::HandleIncrementTeamScore() {
    AFlagnadoGameState *FlagnadoGameState = GetWorld()->GetGameState<AFlagnadoGameState>();
    FLAGNADO_RETURN_IF(!FlagnadoGameState);

    FlagnadoGameState->AddTeamOrIncrementTeamScore(Team);
}
