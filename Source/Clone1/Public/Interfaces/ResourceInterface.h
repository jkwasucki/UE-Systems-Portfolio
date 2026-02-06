#pragma once
#include "CoreMinimal.h"
#include "ResourceInterface.generated.h"


UINTERFACE(BlueprintType)
class UResourceInterface : public UInterface
{
	GENERATED_BODY()
};

class IResourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	float GetHealth() const;
	UFUNCTION(BlueprintNativeEvent)
	void ModifyHealth(float Delta);

	UFUNCTION(BlueprintNativeEvent)
	float GetEnergy() const;
	UFUNCTION(BlueprintNativeEvent)
	void ModifyEnergy(float Delta);
};
