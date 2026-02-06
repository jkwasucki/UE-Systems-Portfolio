// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Misc/AbilityProjectile.h"
#include "AbilitySystem/Definitions/AbilityEffect.h"
#include "Structs/FAbilityProjectileData.h"
#include "ProjectileEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew,DefaultToInstanced)
class CLONE1_API UProjectileEffect : public UAbilityEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAbilityProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAbilityProjectileData ProjectileData;
	
	virtual bool TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData, FGuid AbilityInstanceID) override;
	
};
