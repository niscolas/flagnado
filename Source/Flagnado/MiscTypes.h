#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include "MiscTypes.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8 { None = -1, Red = 0, Blue = 1, Green = 2, Yellow = 3, Purple = 4 };

UCLASS(BlueprintType)

class FLAGNADO_API UTeamData : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tem Data")
    FColor Color;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tem Data")
    int32 CurrentScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tem Data")
    int32 TargetScore;
};
