#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "FlagnadoFlag.generated.h"

class USphereComponent;

UCLASS()

class FLAGNADO_API AFlagnadoFlag : public AActor, public IGameplayTagAssetInterface {
    GENERATED_BODY()

public:
    AFlagnadoFlag();

    virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;

    UFUNCTION(BlueprintCallable, Category = "Flagnado|Flag")
    void OnPickedUp();

    UFUNCTION(BlueprintCallable, Category = "Flagnado|Flag")
    void OnDropped();

private:
    virtual void BeginPlay() override;

    UPROPERTY(ReplicatedUsing = OnRep_CollisionState)
    bool ShouldEnableCollision = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    UStaticMeshComponent *Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
    USphereComponent *SphereCollider;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
    FGameplayTagContainer GameplayTags;

    UFUNCTION()
    void OnRep_CollisionState();

    virtual void
    GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    void SetCollisionToDefault();
    void DisableCollision();
};
