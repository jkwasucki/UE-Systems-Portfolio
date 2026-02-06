#pragma once
#include "CoreMinimal.h"
#include "DamageableInterface.generated.h"


UINTERFACE(BlueprintType)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class IDamageableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void TakeDamage(float Value);
};
