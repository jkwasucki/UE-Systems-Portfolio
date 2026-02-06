#pragma once
#include "CoreMinimal.h"
#include "EffectTypes.generated.h"



UENUM(BlueprintType)
enum class EEffectSource : uint8
{
	None,
	Ability,
	Consumable,
};


UENUM(BlueprintType)
enum class EEffectType : uint8
{
	None,
	RestoreResource,
	DecreaseResource,
	IncreaseAttrbute,
	DecreaseAttrbute,
};

UENUM(BlueprintType)
enum class EEffectMode : uint8
{
	None,
	InstantPersistent, // Applied instantly once, effect stays
	InstantDuration, // Applied instantly once, effect is removed after x seconds
	Overtime, // Applies x value over y seconds
};

