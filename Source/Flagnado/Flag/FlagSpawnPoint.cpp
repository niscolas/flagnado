#include "FlagSpawnPoint.h"
#include "Components/BillboardComponent.h"

AFlagSpawnPoint::AFlagSpawnPoint() {
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

#if WITH_EDITORONLY_DATA
    EditorIconComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
    EditorIconComponent->SetupAttachment(RootComponent);
#endif

    SpawnLocationReferenceComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationReference"));
    SpawnLocationReferenceComponent->SetRelativeLocation(FVector(0, 0, 50.f));
    SpawnLocationReferenceComponent->SetupAttachment(RootComponent);
}

void AFlagSpawnPoint::BeginPlay() {
    Super::BeginPlay();
}
