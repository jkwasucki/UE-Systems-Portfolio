#pragma once
#include "CoreMinimal.h"
#include "AttributeTypes.generated.h"


UENUM(BlueprintType)
enum class EAttribute : uint8
{
	None,
	Armor,
	Health,
	Speed,
	Attack
};
