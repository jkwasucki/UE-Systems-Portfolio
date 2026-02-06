// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FAbilityIndicatorData.h"
#include "Structs/FAbilityTargetData.h"
#include "Structs/FCustomTargetingRules.h"
#include "Types/AbilityTypes.h"
#include "UObject/NoExportTypes.h"
#include "TargetingStrategy.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CLONE1_API UTargetingStrategy : public UObject
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintNativeEvent)	
	FAbilityTargetData GetValidTargets(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent)
	EAbilityTargetingStatus TryResolveTargeting(AActor* Instigator,FAbilityIndicatorData& IndicatorData,FCustomTargetingRules& CustomTargetingRules, FAbilityTargetData& iOutTargetData);
	UFUNCTION(BlueprintNativeEvent)
	void AbortTargeting();
	UFUNCTION(BlueprintNativeEvent)
	void TickTargeting(float DeltaTime);

};
