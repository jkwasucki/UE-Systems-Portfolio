#pragma once
#include "CoreMinimal.h"
#include "SavableInterface.generated.h"

UINTERFACE(BlueprintType)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
	
};
class  ISavableInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	FName GetSaveID() const;
	UFUNCTION(BlueprintNativeEvent)
	void SerializeToBinary(TArray<uint8>&  OutData) const;
	UFUNCTION(BlueprintNativeEvent)
	void DeserializeFromBinary(const TArray<uint8>& InData);
};