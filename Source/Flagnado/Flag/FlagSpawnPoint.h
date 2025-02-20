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

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    UBillboardComponent *EditorIconComponent;
};
