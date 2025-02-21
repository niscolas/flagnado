#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
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

    static FString GetNetModeString(UWorld *World) {
        if (!World)
            return TEXT("INVALID WORLD");

        switch (World->GetNetMode()) {
        case NM_Client:
            return TEXT("CLIENT");
        case NM_ListenServer:
            return TEXT("LISTEN SERVER");
        case NM_DedicatedServer:
            return TEXT("DEDICATED SERVER");
        case NM_Standalone:
            return TEXT("STANDALONE");
        default:
            return TEXT("UNKNOWN");
        }
    }
};
