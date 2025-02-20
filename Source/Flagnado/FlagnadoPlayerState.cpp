#include "FlagnadoPlayerState.h"
#include "HelperMacros.h"
#include "MiscTypes.h"

AFlagnadoPlayerState::AFlagnadoPlayerState() {
    CurrentTeam = ETeam::None;
}

void AFlagnadoPlayerState::SetTeam(ETeam InTeam) {
    FLAGNADO_RETURN_IF(!HasAuthority());

    CurrentTeam = InTeam;
}

void AFlagnadoPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
