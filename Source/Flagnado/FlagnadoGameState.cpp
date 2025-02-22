#include "FlagnadoGameState.h"
#include "Flagnado/FlagnadoHelpers.h"
#include "Flagnado/HelperMacros.h"
#include "HelperMacros.h"
#include "MiscTypes.h"
#include "Net/UnrealNetwork.h"
#include "TeamsColorProfileDataAsset.h"

AFlagnadoGameState::AFlagnadoGameState() {
}

void AFlagnadoGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFlagnadoGameState, TeamScoresKeys);
    DOREPLIFETIME(AFlagnadoGameState, TeamScoresValues);
}

void AFlagnadoGameState::AddTeamOrIncrementTeamScore(ETeam InTeam) {
    int32 TeamIndex = TeamScoresKeys.Find(InTeam);

    if (TeamIndex == INDEX_NONE) {
        TeamScoresKeys.Add(InTeam);
        TeamScoresValues.Add(1);

    } else {
        TeamScoresValues[TeamIndex]++;
    }

    if (HasAuthority()) {
        BroadcastAnyTeamDataChanged();
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

void AFlagnadoGameState::OnRep_TeamScoresValues() {
    BroadcastAnyTeamDataChanged();
}

void AFlagnadoGameState::BroadcastAnyTeamDataChanged() {
    AnyTeamDataChanged.Broadcast();
}

TArray<UTeamData *> AFlagnadoGameState::GetTeamsData() const {
    TArray<UTeamData *> Result;
    for (int32 i = 0; i < TeamScoresKeys.Num(); i++) {
        UTeamData *TeamData = NewObject<UTeamData>();

        if (TeamsColorProfileDataAsset) {
            TeamData->Color = TeamsColorProfileDataAsset->GetColorForTeam(TeamScoresKeys[i]);
        }

        TeamData->CurrentScore = TeamScoresValues[i];
        TeamData->TargetScore = 3;
        Result.Add(TeamData);

        UE_LOG(LogTemp, Warning, TEXT("(%s) Team: %s, %d, %d"),
               *UFlagnadoHelpers::GetNetModeString(GetWorld()), *TeamData->Color.ToString(),
               TeamData->CurrentScore, TeamData->TargetScore);
    }

    return Result;
}
