#pragma once
#include "CoreMinimal.h"
#include "FComponentDataBinary.h"
#include "FPlayerSaveData.generated.h"

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FName,FComponentBinaryData> ComponentData;
};
