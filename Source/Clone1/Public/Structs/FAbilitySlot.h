#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Definitions/AbilityData.h"
#include "FAbilitySlot.generated.h"

USTRUCT(BlueprintType)
struct FFAbilitySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag SlotTag;
	
	UPROPERTY(EditAnywhere)
	UAbilityData* AbilityData;
};
