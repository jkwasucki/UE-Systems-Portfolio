// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/ReviveEffect.h"

#include "Main/Character/Base/BaseCharacter.h"

bool UReviveEffect::TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData,
                                               FGuid AbilityInstanceID)
{
	if (ABaseCharacter* Char = Cast<ABaseCharacter>(TargetData.TargetActors[0]))
	{
		if (Char->IsAlive())
		{
			return false;
		}
	}
		
	return Super::TryApplyEffect_Implementation(Instigator, TargetData, AbilityInstanceID);
}
