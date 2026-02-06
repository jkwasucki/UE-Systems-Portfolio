// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Misc/AbilityAreaIndicator.h"
#include "AbilitySystem/Definitions/TargetingStrategy.h"
#include "AOEManualTargetingStrategy.generated.h"

class AMainPlayerController;
/**
 * 
 */
UCLASS()
class CLONE1_API UAoeManualTargetingStrategy : public UTargetingStrategy
{
	GENERATED_BODY()
protected:
	
	UPROPERTY()
	bool bIsTargeting = false;
	UPROPERTY()
	AAbilityAreaIndicator* Indicator = nullptr;
	UPROPERTY()
	AMainPlayerController* PC = nullptr;
	UPROPERTY()
	FVector CachedDesiredLocation;
public:
	
	virtual EAbilityTargetingStatus TryResolveTargeting_Implementation(AActor* Instigator, FAbilityIndicatorData& IndicatorData,  FCustomTargetingRules& CustomTargetingRules,FAbilityTargetData& OutTargetData) override;
	virtual void TickTargeting_Implementation(float DeltaTime) override;
	virtual void AbortTargeting_Implementation() override;
	UFUNCTION()
	void StartTargeting(FAbilityIndicatorData& IndicatorData, AMainPlayerController* inPC);
	UFUNCTION()
	FAbilityTargetData EndTargeting();
	UFUNCTION()
	void SpawnIndicator(FAbilityIndicatorData& IndicatorData);
};
