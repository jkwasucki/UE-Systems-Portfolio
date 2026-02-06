#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Misc/AbilityAreaIndicator.h"
#include "FAbilityIndicatorData.generated.h"


USTRUCT(BlueprintType)
struct FAbilityIndicatorData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAbilityAreaIndicator> IndicatorClass;
};
