#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "TP_WeaponComponent.generated.h"

class AFlagnadoCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UTP_WeaponComponent : public USkeletalMeshComponent {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AFlagnadoProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    USoundBase *FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    UAnimMontage *FireAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext *FireMappingContext;

    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputAction *FireAction;

    UTP_WeaponComponent();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool AttachWeapon(AFlagnadoCharacter *TargetCharacter);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void PlayFireFX();

private:
    AFlagnadoCharacter *Character;

    UFUNCTION()
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
