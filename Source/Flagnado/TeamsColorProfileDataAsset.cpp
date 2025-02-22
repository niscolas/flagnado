#include "TeamsColorProfileDataAsset.h"
#include "FlagnadoHelpers.h"

UTeamsColorProfileDataAsset::UTeamsColorProfileDataAsset() {
}

UMaterialInterface *UTeamsColorProfileDataAsset::GetMaterialForTeam(ETeam InTeam) const {
    return *TeamsMaterials.Find(InTeam);
}

FColor UTeamsColorProfileDataAsset::GetColorForTeam(ETeam InTeam) const {
    return *TeamsColors.Find(InTeam);
}
