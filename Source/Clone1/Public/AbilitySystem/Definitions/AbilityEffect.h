// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FAbilityEffectsDescriptor.h"
#include "Structs/FAbilityTargetData.h"
#include "Structs/FCharacterEffect.h"
#include "Types/AbilityTypes.h"
#include "UObject/NoExportTypes.h"
#include "AbilityEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew,DefaultToInstanced)
class CLONE1_API UAbilityEffect : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterEffect EffectData;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbilityEffect>> InflictedOnHitEffects;
	
	UFUNCTION(BlueprintNativeEvent)
	bool TryApplyEffect(AActor* Instigator,FAbilityTargetData& TargetData, FGuid AbilityInstanceID);
	UFUNCTION()
	virtual FCharacterEffect GetEffectData();
	UFUNCTION()
	virtual TArray<TSubclassOf<UAbilityEffect>>& GetInnerEffects();
};
