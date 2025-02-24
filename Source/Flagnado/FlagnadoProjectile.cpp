#include "FlagnadoProjectile.h"
#include "Components/SphereComponent.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HelperMacros.h"

AFlagnadoProjectile::AFlagnadoProjectile() {
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AFlagnadoProjectile::OnHit);

    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    RootComponent = CollisionComp;

    ProjectileMovement =
        CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    InitialLifeSpan = 3.f;
}

void AFlagnadoProjectile::OnHit(UPrimitiveComponent *HitComp,
                                AActor *OtherActor,
                                UPrimitiveComponent *OtherComp,
                                FVector NormalImpulse,
                                const FHitResult &Hit) {
    FLAGNADO_RETURN_IF(!OtherActor || OtherActor == this || !OtherComp || OtherActor == Owner);
    FLAGNADO_RETURN_IF(UFlagnadoHelpers::CheckHaveSameTeam(Owner, OtherActor));

    AFlagnadoCharacter *PossibleFlagnadoCharacter = Cast<AFlagnadoCharacter>(OtherActor);
    FLAGNADO_RETURN_IF(!PossibleFlagnadoCharacter);

    if (HasAuthority()) {
        PossibleFlagnadoCharacter->OnShot();
    }

    Destroy();
}
