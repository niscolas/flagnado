#include "TeamMemberSpawnPoint.h"
#include "Components/BillboardComponent.h"
#include "HelperMacros.h"
#include "TeamsColorProfileDataAsset.h"

ATeamMemberSpawnPoint::ATeamMemberSpawnPoint() {
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = Mesh;

    SpawnLocationReferenceComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationReference"));
    SpawnLocationReferenceComponent->SetRelativeLocation(FVector(0, 0, 200.f));
    SpawnLocationReferenceComponent->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
    EditorIconComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
    EditorIconComponent->SetupAttachment(RootComponent);
#endif
}

void ATeamMemberSpawnPoint::BeginPlay() {
    Super::BeginPlay();

    UpdateMeshColor();
}

ETeam ATeamMemberSpawnPoint::GetTeam() const {
    return Team;
}

void ATeamMemberSpawnPoint::UpdateMeshColor() {
    FLAGNADO_RETURN_IF(!TeamsColorProfileDataAsset);

    UMaterialInterface *TeamMaterial = TeamsColorProfileDataAsset->GetMaterialForTeam(Team);
    FLAGNADO_RETURN_IF(!TeamMaterial);

    Mesh->SetMaterial(0, TeamMaterial);
}
