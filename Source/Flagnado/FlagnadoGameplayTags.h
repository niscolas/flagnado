#pragma once

#include "NativeGameplayTags.h"

namespace FlagnadoGameplayTags {
FLAGNADO_API
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_PickFlag);

FLAGNADO_API
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_FlagPickup);

FLAGNADO_API
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_HoldingTheFlag);

FLAGNADO_API
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_PickFlag);

FLAGNADO_API
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Pickable);
} // namespace FlagnadoGameplayTags
