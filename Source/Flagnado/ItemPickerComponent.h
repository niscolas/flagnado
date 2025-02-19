#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ItemPickerComponent.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UItemPickerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UItemPickerComponent();

private:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Flagnado|Ability System",
              meta = (AllowPrivateAccess))
    UAbilitySystemComponent *OwnerAbilitySystemComponent;

    UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess))
    void OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);
};
