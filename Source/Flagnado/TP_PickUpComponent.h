#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "FlagnadoCharacter.h"
#include "TP_PickUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp,
                                            AFlagnadoCharacter *,
                                            PickUpCharacter);

UCLASS(Blueprintable,
       BlueprintType,
       ClassGroup = (Custom),
       meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UTP_PickUpComponent : public USphereComponent {
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Flagnado|Interaction")
    FOnPickUp OnPickUp;

    UTP_PickUpComponent();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor,
                              UPrimitiveComponent *OtherComp,
                              int32 OtherBodyIndex,
                              bool bFromSweep,
                              const FHitResult &SweepResult);
};
