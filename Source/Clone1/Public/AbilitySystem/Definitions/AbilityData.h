// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "GameplayTagContainer.h"
#include "TargetingStrategy.h"
#include "Structs/FAbilityIndicatorData.h"
#include "Structs/FAnimationData.h"
#include "Structs/FVFXData.h"
#include "Types/AbilityTypes.h"
#include "AbilityData.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UAbilityData();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag Tag;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EAbilityCastMode AbilityCastMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (
		EditCondition = "AbilityCastMode == EAbilityCastMode::Casted",
		EditConditionHides
	))
	float CastTime = 0;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float Cooldown;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 EnergyCost;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FAbilityIndicatorData IndicatorData;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FCustomTargetingRules CustomTargetingRules;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UTargetingStrategy> TargetingStrategy;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbilityEffect>> Effects;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FAnimationData AnimationData;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FVFXData> VFXData;
};
