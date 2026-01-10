#pragma once
#include "FItemStack.generated.h"

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;
	
	
	FItemStack(){};
	FItemStack(FName inItemName, int32 inAmount) : ItemID(inItemName)	, Amount(inAmount){};
	
	void Reset()
	{
		ItemID = NAME_None;
		Amount = 1;
	}
};
