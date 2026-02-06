// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Definitions/AbilityEffect.h"

#include "Main/Character/Base/BaseCharacter.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Main/Character/EffectsComponent.h"

bool UAbilityEffect::TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData,FGuid AbilityInstanceID)
{
	if (!Instigator) return false;
	UWorld* World = Instigator->GetWorld();
	if (!World) return false;
	
	ABaseCharacter* Char = Cast<ABaseCharacter>(Instigator->GetInstigator());
	if (!Char) return false;
	
	for (AActor* Target : TargetData.TargetActors)
	{
		if (Target->Implements<UCharacterEffectReceiverInterface>())
		{
			ICharacterEffectReceiverInterface::Execute_ApplyEffect(Target, Instigator,EffectData,AbilityInstanceID);
			return true;
		}
		return false;
	}
	return false;
}

FCharacterEffect UAbilityEffect::GetEffectData()
{
	return EffectData;
}

TArray<TSubclassOf<UAbilityEffect>>& UAbilityEffect::GetInnerEffects()
{
	return InflictedOnHitEffects;
}
