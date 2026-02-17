#pragma once
#include "CoreMinimal.h"
#include "FInventoryItemSaveData.generated.h"

USTRUCT()
struct FInventoryItemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ItemID;
	UPROPERTY()
	int32 Quantity;
	
	bool Serialize(FArchive& Ar)
	{
		Ar << ItemID;;
		Ar << Quantity;
		return true;
	}
	
	friend FArchive& operator<<(FArchive& Ar, FInventoryItemSaveData& Data)
	{
		Data.Serialize(Ar);
		return Ar;
	}
};
