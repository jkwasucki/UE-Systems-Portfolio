#pragma once
#include "CoreMinimal.h"
#include "FPlayerSaveData.h"
#include "FGameSaveData.generated.h"

USTRUCT()
struct FGameSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 GlobalVersion = 1;
	UPROPERTY()
	FPlayerSaveData PlayerData;
};
