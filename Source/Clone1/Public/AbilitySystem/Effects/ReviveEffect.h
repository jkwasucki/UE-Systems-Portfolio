// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Definitions/AbilityEffect.h"
#include "ReviveEffect.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UReviveEffect : public UAbilityEffect
{
	GENERATED_BODY()
public:
	virtual bool TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData, FGuid AbilityInstanceID) override;
};
