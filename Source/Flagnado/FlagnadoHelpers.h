#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FlagnadoHelpers.generated.h"

class UWorld;

UCLASS()

class UFlagnadoHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    template <typename T>
    static T *GetGameMode(UWorld *World) {
        return World ? Cast<T>(World->GetAuthGameMode()) : nullptr;
    }
};
