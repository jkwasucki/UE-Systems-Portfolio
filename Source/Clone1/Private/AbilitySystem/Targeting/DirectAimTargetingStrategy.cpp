// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Targeting/DirectAimTargetingStrategy.h"

#include "Interfaces/CharacterEffectReciverInterface.h"

EAbilityTargetingStatus
UDirectAimTargetingStrategy::TryResolveTargeting_Implementation(
	AActor* Instigator,
	FAbilityIndicatorData& IndicatorData,
	FCustomTargetingRules& CustomTargetingRules,
	FAbilityTargetData& OutTargetData)
{
	if (!Instigator)
	{;
		return EAbilityTargetingStatus::Invalid;
	}

	APawn* Pawn = Cast<APawn>(Instigator);
	if (!Pawn)
	{
		return EAbilityTargetingStatus::Invalid;
	}

	AController* Controller = Pawn->GetController();
	if (!Controller)
	{
		return EAbilityTargetingStatus::Invalid;
	}
	
	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector AimDirection = ViewRotation.Vector();
	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + AimDirection * 10000.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Instigator);
	Params.bTraceComplex = true;
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	
	const bool bHit = Instigator->GetWorld()->LineTraceSingleByObjectType(
		Hit,
		TraceStart,
		TraceEnd,
		ObjectParams,
		Params
	);

	// Always fill these
	OutTargetData.Direction = AimDirection;

	AActor* HitActor = Hit.GetActor();
	
	if (!HitActor)
	{	
		return EAbilityTargetingStatus::Invalid;
	}
		


	if (bHit)
	{
		OutTargetData.TargetLocation = Hit.ImpactPoint;
		if (HitActor->Implements<UCharacterEffectReceiverInterface>())
		{
			OutTargetData.TargetActors = { HitActor };
			OutTargetData.bHasValidTarget = true;

			return EAbilityTargetingStatus::Confirmed;
		}
	}
	return EAbilityTargetingStatus::Invalid;
}
