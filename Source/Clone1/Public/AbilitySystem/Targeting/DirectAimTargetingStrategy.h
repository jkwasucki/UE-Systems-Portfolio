// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Definitions/TargetingStrategy.h"
#include "DirectAimTargetingStrategy.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UDirectAimTargetingStrategy : public UTargetingStrategy
{
	GENERATED_BODY()
public:
	virtual EAbilityTargetingStatus TryResolveTargeting_Implementation(AActor* Instigator, FAbilityIndicatorData& IndicatorData, FCustomTargetingRules& CustomTargetingRules, FAbilityTargetData& OutTargetData) override;
};
