#pragma once
#include "CoreMinimal.h"
#include "FComponentDataBinary.generated.h"

USTRUCT()
struct FComponentBinaryData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> Data;
};