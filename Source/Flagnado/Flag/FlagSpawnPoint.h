#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagSpawnPoint.generated.h"

class UBillboardComponent;

UCLASS()

class FLAGNADO_API AFlagSpawnPoint : public AActor {
    GENERATED_BODY()

public:
    AFlagSpawnPoint();

    FVector GetSpawnLocation() const {
        return SpawnLocationReferenceComponent->GetComponentLocation();
    }

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    UBillboardComponent *EditorIconComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    USceneComponent *SpawnLocationReferenceComponent;
};
