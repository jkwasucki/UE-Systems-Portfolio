// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/AreaEffect.h"
#include "AbilitySystem/Misc/AbilityAreaOccurence.h"
#include "Main/Character/Derived/MainCharacter.h"

bool UAreaEffect::TryApplyEffect_Implementation(AActor* Instigator, FAbilityTargetData& TargetData,
                                             FGuid AbilityInstanceID)
{
		if (!Instigator) return false;
		UWorld* World = Instigator->GetWorld();
		if (!World) return false;
	
		AMainCharacter* Char = Cast<AMainCharacter>(Instigator->GetInstigator());
		if (!Char) return false;

		FTransform SpawnTransform = FTransform(TargetData.TargetLocation);
		FActorSpawnParameters Params;
		Params.Owner = Char;
		Params.Instigator = Char;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAbilityAreaOccurence* Occurence =
			World->SpawnActor<AAbilityAreaOccurence>(
				AreaOccurenceClass,
				SpawnTransform,
				Params
			);
	
		if (!Occurence) return false;
	
		AreaOccurenceData.InflictedOnHitEffects = InflictedOnHitEffects;
		
		Occurence->InitializeAreaOccurence(Instigator, AreaOccurenceData, TargetData.TargetLocation);
	return true;
}
