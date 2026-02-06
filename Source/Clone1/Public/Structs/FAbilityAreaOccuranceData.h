#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Definitions/AbilityEffect.h"
#include "FAbilityAreaOccuranceData.generated.h"


USTRUCT(BlueprintType)
struct FAbilityAreaOccurenceData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<UAbilityEffect>> InflictedOnHitEffects;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FVFXData AreaVFX;
};
