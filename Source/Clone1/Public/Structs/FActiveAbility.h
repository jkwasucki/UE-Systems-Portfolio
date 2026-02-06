#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FActiveAbility.generated.h"

USTRUCT(BlueprintType)
struct FActiveAbility
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGuid AbilityInstanceID;
	
};
