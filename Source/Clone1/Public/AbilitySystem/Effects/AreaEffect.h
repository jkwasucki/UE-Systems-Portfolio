// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Misc/AbilityAreaOccurence.h"
#include "AbilitySystem/Definitions/AbilityEffect.h"
#include "Structs/FAbilityAreaOccuranceData.h"
#include "AreaEffect.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UAreaEffect : public UAbilityEffect
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAbilityAreaOccurence> AreaOccurenceClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAbilityAreaOccurenceData AreaOccurenceData;
	
	virtual bool TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData, FGuid AbilityInstanceID) override;
	
};
