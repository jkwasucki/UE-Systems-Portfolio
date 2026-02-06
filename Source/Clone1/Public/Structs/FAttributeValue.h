#pragma once
#include "CoreMinimal.h"
#include "FAttributeValue.generated.h"

USTRUCT(BlueprintType)
struct FFAttributeValue
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BaseValue = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BonusValue = 0.0f;
	
	float GetFinal() const { return BaseValue + BonusValue; };
};
