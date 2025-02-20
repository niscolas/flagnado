#pragma once

#include "CoreMinimal.h"
#include "MiscTypes.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8 {
    None = -1,
    Red = 0,
    Blue = 1,
    Green = 2,
    Yellow = 3,
    Purple = 4
};
