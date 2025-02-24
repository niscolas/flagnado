#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Flagnado/FlagnadoGameplayTags.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "HelperMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogVerbosity.h"
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

    static bool NativeCheckDoesActorHaveTag(AActor *InActor, FGameplayTag InTag) {
        UAbilitySystemComponent *AbilitySystemComponent =
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
        return AbilitySystemComponent->HasMatchingGameplayTag(InTag);
    }

    static bool CheckHaveSameTeam(AActor *ActorA, AActor *ActorB) {
        FLAGNADO_LOG_AND_RETURN_VALUE_IF(!ActorA || !ActorB, false, LogTemp, Error,
                                         TEXT("Invalid Actors"));

        UAbilitySystemComponent *ASC_A =
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorA);
        UAbilitySystemComponent *ASC_B =
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorB);

        FLAGNADO_RETURN_VALUE_IF(!ASC_A || !ASC_B, false);

        FGameplayTagContainer ActorATags, ActorBTags;
        ASC_A->GetOwnedGameplayTags(ActorATags);
        ASC_B->GetOwnedGameplayTags(ActorBTags);

        for (const FGameplayTag &TagA : ActorATags) {
            FLAGNADO_CONTINUE_IF(!TagA.MatchesTag(FlagnadoGameplayTags::Player_Team));
            FLAGNADO_CONTINUE_IF(!ActorBTags.HasTagExact(TagA));
            return true;
        }

        return false;
    }
};
