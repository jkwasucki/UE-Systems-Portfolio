// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Definitions/ForwardAimTargetingStrategy.h"

EAbilityTargetingStatus
UForwardAimTargetingStrategy::TryResolveTargeting_Implementation(
	AActor* Instigator,
	FAbilityIndicatorData& IndicatorData,
	FCustomTargetingRules& CustomTargetingRules,
	FAbilityTargetData& OutTargetData)
{
	if (!Instigator)
		return EAbilityTargetingStatus::Invalid;

	OutTargetData.Direction = Instigator->GetActorForwardVector();
	OutTargetData.bHasValidTarget = true;

	// Debug
	DrawDebugLine(
		Instigator->GetWorld(),
		Instigator->GetActorLocation(),
		Instigator->GetActorLocation() + OutTargetData.Direction * 2000.f,
		FColor::Red,
		false,
		2.f
	);

	return EAbilityTargetingStatus::Confirmed;
}