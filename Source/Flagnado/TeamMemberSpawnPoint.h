#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiscTypes.h"
#include "TeamMemberSpawnPoint.generated.h"

class UTeamsColorProfileDataAsset;
class UBillboardComponent;
class USceneComponent;

UCLASS()

class FLAGNADO_API ATeamMemberSpawnPoint : public AActor {
    GENERATED_BODY()

public:
    ATeamMemberSpawnPoint();

    UFUNCTION(BlueprintPure, Category = "Flagnado|Team Member Spawn Point")
    ETeam GetTeam() const;

    FVector GetSpawnLocation() const {
        return SpawnLocationReferenceComponent->GetComponentLocation();
    }

private:
    UPROPERTY(EditAnywhere,
              BlueprintReadOnly,
              Category = "Team Member Spawn Point",
              meta = (AllowPrivateAccess))
    ETeam Team;

    UPROPERTY(EditDefaultsOnly,
              BlueprintReadOnly,
              Category = "Team Member Spawn Point",
              meta = (AllowPrivateAccess))
    UTeamsColorProfileDataAsset *TeamsColorProfileDataAsset;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Team Member Spawn Point|Debug",
              meta = (AllowPrivateAccess))
    UStaticMeshComponent *Mesh;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Team Member Spawn Point|Debug",
              meta = (AllowPrivateAccess))
    UBillboardComponent *EditorIconComponent;

    UPROPERTY(VisibleAnywhere,
              BlueprintReadOnly,
              Category = "Team Member Spawn Point|Debug",
              meta = (AllowPrivateAccess))
    USceneComponent *SpawnLocationReferenceComponent;

    virtual void BeginPlay() override;

    void UpdateMeshColor();
};
