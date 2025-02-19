#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "FlagnadoFlag.generated.h"

class USphereComponent;

UCLASS()

class FLAGNADO_API AFlagnadoFlag : public AActor,
                                   public IGameplayTagAssetInterface {
    GENERATED_BODY()

public:
    AFlagnadoFlag();

    virtual void
    GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    UStaticMeshComponent *Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    USphereComponent *SphereCollider;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
    FGameplayTagContainer GameplayTags;
};
