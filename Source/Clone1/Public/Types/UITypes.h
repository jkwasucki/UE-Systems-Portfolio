#pragma once
#include "CoreMinimal.h"
#include "UITypes.generated.h"


UENUM(BlueprintType)
enum class EInventoryContextAction : uint8
{
	None,
	Equip,
	Consume,
	Split
};

