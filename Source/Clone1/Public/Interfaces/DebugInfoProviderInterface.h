#pragma once
#include "Structs/FGameplayDebugSnapshot.h"
#include "DebugInfoProviderInterface.generated.h"

UINTERFACE(BlueprintType)
class UDebugInfoProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class IDebugInfoProviderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FEntityGameplayDebugSnapshot GetDebugInfo();
};