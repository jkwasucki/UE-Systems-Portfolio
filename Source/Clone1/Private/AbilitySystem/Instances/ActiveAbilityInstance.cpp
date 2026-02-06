// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Instances/ActiveAbilityInstance.h"

void UActiveAbilityInstance::StartCast(float Duration)
{
	if (!GetWorld())
	{
		return;
	}

	if (Duration <= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				OnCastFinishedDelegate.Broadcast();
			})
		);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		CastTimerHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			OnCastFinishedDelegate.Broadcast();
		}),
		Duration,
		false
	);
}


void UActiveAbilityInstance::CancelCast()
{
	GetWorld()->GetTimerManager().ClearTimer(CastTimerHandle);
}
