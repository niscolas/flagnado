#include "FlagnadoGameState.h"

AFlagnadoGameState::AFlagnadoGameState() {
}

void AFlagnadoGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AFlagnadoGameState::AddTeamOrIncrementTeamScore(ETeam InTeam) {
    int32 TeamIndex = TeamScoresKeys.Find(InTeam);

    if (TeamIndex == INDEX_NONE) {
        TeamScoresKeys.Add(InTeam);
        TeamScoresValues.Add(1);
    } else {
        TeamScoresValues[TeamIndex]++;
    }
}

int32 AFlagnadoGameState::GetTeamScore(ETeam InTeam) const {
    int32 TeamIndex = TeamScoresKeys.Find(InTeam);

    if (TeamIndex == INDEX_NONE) {
        return 0;
    } else {
        return TeamScoresValues[TeamIndex];
    }
}

UMaterialInterface *AFlagnadoGameState::GetMaterialForTeam(ETeam InTeam) const {
    int32 TeamIndex = TeamMaterialsKeys.Find(InTeam);

    if (TeamIndex == INDEX_NONE) {
        return nullptr;
    } else {
        return TeamMaterialsValues[TeamIndex];
    }
}
