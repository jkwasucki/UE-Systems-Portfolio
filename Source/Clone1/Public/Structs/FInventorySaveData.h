#pragma once
#include "CoreMinimal.h"
#include "FInventoryItemSaveData.h"
#include "FInventorySaveData.generated.h"



USTRUCT()
struct FInventorySaveData
{
	GENERATED_BODY()
	UPROPERTY()
	int32 Version = 1;
	UPROPERTY()
	TArray<FInventoryItemSaveData> Items;
	
	
	bool Serialize(FArchive& Ar)
	{
		Ar << Version;
		Ar << Items;
		return true;
	}
	
	friend FArchive& operator<<(FArchive& Ar, FInventorySaveData& Data)
	{
		Data.Serialize(Ar);
		return Ar;
	}
};
