// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Targeting/SelfTargetingStrategy.h"



EAbilityTargetingStatus USelfTargetingStrategy::TryResolveTargeting_Implementation(AActor* Instigator,
	FAbilityIndicatorData& IndicatorData,  FCustomTargetingRules& CustomTargetingRules,FAbilityTargetData& OutTargetData)
{
	FAbilityTargetData TargetData;
	
	TArray<AActor*> Targets;
	Targets.Add(Instigator);
	
	TargetData.TargetActors = Targets;
	TargetData.bHasValidTarget = true;
	OutTargetData = TargetData;
	return EAbilityTargetingStatus::Confirmed;
}
