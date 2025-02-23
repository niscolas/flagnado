#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagnadoProjectile.generated.h"

class AFlagnadoCharacter;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()

class FLAGNADO_API AFlagnadoProjectile : public AActor {
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent *CollisionComp;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = Movement,
              meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent *ProjectileMovement;

public:
    AFlagnadoProjectile();

    UFUNCTION()
    void OnHit(UPrimitiveComponent *HitComp,
               AActor *OtherActor,
               UPrimitiveComponent *OtherComp,
               FVector NormalImpulse,
               const FHitResult &Hit);

    USphereComponent *GetCollisionComp() const {
        return CollisionComp;
    }

    UProjectileMovementComponent *GetProjectileMovement() const {
        return ProjectileMovement;
    }
};
