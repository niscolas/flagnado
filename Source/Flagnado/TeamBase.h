#pragma once

#include "CoreMinimal.h"
#include "Flagnado/MiscTypes.h"
#include "GameFramework/Actor.h"
#include "TeamBase.generated.h"

class USphereComponent;

UCLASS()

class FLAGNADO_API ATeamBase : public AActor {
    GENERATED_BODY()

public:
    ATeamBase();

    UFUNCTION(BlueprintPure, Category = "Flagnado|Team Base")
    ETeam GetTeam() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team Base", meta = (AllowPrivateAccess))
    ETeam Team;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Team Base|Debug",
              meta = (AllowPrivateAccess))
    UStaticMeshComponent *Mesh;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Team Base|Debug",
              meta = (AllowPrivateAccess))
    USphereComponent *SphereCollider;

    UFUNCTION(meta = (AllowPrivateAccess))
    void OnBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);

    virtual void BeginPlay() override;

    void UpdateMeshColor();

    void IncrementTeamScore(AActor *PossibleTeamMember);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_IncrementTeamScore();

    void HandleIncrementTeamScore();
};
