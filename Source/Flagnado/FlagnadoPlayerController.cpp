#include "FlagnadoPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

void AFlagnadoPlayerController::BeginPlay() {
    Super::BeginPlay();

    // get the enhanced input subsystem
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                GetLocalPlayer())) {
        // add the mapping context so we get controls
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
}
