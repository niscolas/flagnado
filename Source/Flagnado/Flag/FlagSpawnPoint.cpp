#include "FlagSpawnPoint.h"
#include "Components/BillboardComponent.h"

AFlagSpawnPoint::AFlagSpawnPoint() {
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

#if WITH_EDITORONLY_DATA
    EditorIconComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
    EditorIconComponent->SetupAttachment(RootComponent);
#endif
}

void AFlagSpawnPoint::BeginPlay() {
    Super::BeginPlay();
}
