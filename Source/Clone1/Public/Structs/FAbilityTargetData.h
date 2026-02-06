#pragma once
#include "CoreMinimal.h"
#include "FAbilityTargetData.generated.h"

USTRUCT(BlueprintType)
struct FAbilityTargetData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> TargetActors;

	UPROPERTY()
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector Direction = FVector::ZeroVector;

	UPROPERTY()
	bool bHasValidTarget = false;
};