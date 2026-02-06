// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Definitions/TargetingStrategy.h"


FAbilityTargetData UTargetingStrategy::GetValidTargets_Implementation(AActor* Instigator) 
{
	return  FAbilityTargetData();
}

EAbilityTargetingStatus UTargetingStrategy::TryResolveTargeting_Implementation(AActor* Instigator,
	FAbilityIndicatorData& IndicatorData, FCustomTargetingRules& CustomTargetingRules,FAbilityTargetData& iOutTargetData)
{
	return  EAbilityTargetingStatus();
}

void UTargetingStrategy::TickTargeting_Implementation(float DeltaTime)
{
	//
}

void UTargetingStrategy::AbortTargeting_Implementation()
{
	//
}
