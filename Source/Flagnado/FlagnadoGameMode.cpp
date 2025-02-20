#include "FlagnadoGameMode.h"
#include "FlagnadoCharacter.h"
#include "FlagnadoGameState.h"
#include "FlagnadoPlayerState.h"
#include "HelperMacros.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ReflectedTypeAccessors.h"

AFlagnadoGameMode::AFlagnadoGameMode() : Super() {
}

void AFlagnadoGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);

    AFlagnadoPlayerState *FlagnadoPlayerState =
        NewPlayer->GetPlayerState<AFlagnadoPlayerState>();
    FLAGNADO_LOG_AND_RETURN_IF(!FlagnadoPlayerState, LogTemp, Error,
                               TEXT("Will not proceed with PostLogin logic, "
                                    "invalid PlayerState class"));

    int32 NumDifferentTeams = StaticEnum<ETeam>()->NumEnums() - 1;
    ETeam AssignedTeam = static_cast<ETeam>(TeamCounter % NumDifferentTeams);
    TeamCounter++;

    FlagnadoPlayerState->SetTeam(AssignedTeam);
}
