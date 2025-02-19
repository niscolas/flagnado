#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "TP_WeaponComponent.generated.h"

class AFlagnadoCharacter;

UCLASS(Blueprintable,
       BlueprintType,
       ClassGroup = (Custom),
       meta = (BlueprintSpawnableComponent))

class FLAGNADO_API UTP_WeaponComponent : public USkeletalMeshComponent {
    GENERATED_BODY()

public:
    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AFlagnadoProjectile> ProjectileClass;

    /** Sound to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    USoundBase *FireSound;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    UAnimMontage *FireAnimation;

    /** Gun muzzle's offset from the characters location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;

    /** MappingContext */
    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext *FireMappingContext;

    /** Fire Input Action */
    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = Input,
              meta = (AllowPrivateAccess = "true"))
    class UInputAction *FireAction;

    /** Sets default values for this component's properties */
    UTP_WeaponComponent();

    /** Attaches the actor to a FirstPersonCharacter */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool AttachWeapon(AFlagnadoCharacter *TargetCharacter);

    /** Make the weapon Fire a Projectile */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

protected:
    /** Ends gameplay for this component. */
    UFUNCTION()
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    /** The Character holding this weapon*/
    AFlagnadoCharacter *Character;
};
