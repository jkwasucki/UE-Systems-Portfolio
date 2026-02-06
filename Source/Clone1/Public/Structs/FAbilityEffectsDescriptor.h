#pragma once
#include "CoreMinimal.h"
#include "FAbilityEffectsDescriptor.generated.h"

USTRUCT(BlueprintType)
struct FAbilityEffectsDescriptor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FText> Description;
	
};
