#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FlagnadoPlayerController.generated.h"

class UInputMappingContext;

UCLASS()

class FLAGNADO_API AFlagnadoPlayerController : public APlayerController {
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext *InputMappingContext;

protected:
    virtual void BeginPlay() override;
};
