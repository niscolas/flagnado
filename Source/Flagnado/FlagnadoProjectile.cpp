#include "FlagnadoProjectile.h"
#include "Components/SphereComponent.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "FlagnadoCharacter.h"
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

    AFlagnadoCharacter *PossibleFlagnadoCharacter = Cast<AFlagnadoCharacter>(OtherActor);
    FLAGNADO_RETURN_IF(!PossibleFlagnadoCharacter);

    UE_LOG(LogTemp, Warning, TEXT("(%s) Projectile.OnHit %s"),
           *UFlagnadoHelpers::GetNetModeString(GetWorld()), *PossibleFlagnadoCharacter->GetName());

    PossibleFlagnadoCharacter->OnShot();
    Destroy();
}
