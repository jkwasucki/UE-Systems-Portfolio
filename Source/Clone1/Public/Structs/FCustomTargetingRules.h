#pragma once
#include "CoreMinimal.h"
#include "FCustomTargetingRules.generated.h"
USTRUCT(BlueprintType)
struct FCustomTargetingRules
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bAllowDeadTarget;
};
