// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Instances/ActiveEffectInstance.h"
#include "Main/Character/AttributesComponent.h"
#include "Main/Character/Derived/MainCharacter.h"


void UActiveEffectInstance::Initialize(AActor* InEffectOrigin,ABaseCharacter* EffectTarget, FCharacterEffect& CharacterEffect,
                                       FGuid& inSourceInstanceID)
{
	if (!EffectTarget) return;
	CharacterInstance = EffectTarget;
	CharacterEffectDefinition = CharacterEffect;
	SourceInstanceID = inSourceInstanceID;
	EffectInstanceID = FGuid::NewGuid();
	EffectOrigin = InEffectOrigin;
	
	
	SetTimers();
	Apply();
	
}

void UActiveEffectInstance::ApplyOvertimeTick(float Value)
{
	AppliedRawValue += Value;
	CharacterInstance->GetResourceComponent()->UpdateResource(CharacterEffectDefinition.TargetedResource, Value);
	CharacterInstance->GetAttributesComponent()->IncreaseAttribute(CharacterEffectDefinition.TargetedAttribute, Value);
}

void UActiveEffectInstance::Reset()
{
	float remaining = GetWorld()->GetTimerManager().GetTimerRemaining(DurationTick);
		
	float newTime = FMath::Max(remaining, CharacterEffectDefinition.Duration);
	GetWorld()->GetTimerManager().ClearTimer(DurationTick);
		
		
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this,&UActiveEffectInstance::RemoveEffect_Timer);
		
	GetWorld()->GetTimerManager().SetTimer(DurationTick,TimerDelegate, newTime, false);
}

void UActiveEffectInstance::Revert()
{
	CharacterInstance->GetResourceComponent()->UpdateResource(CharacterEffectDefinition.TargetedResource, -CharacterEffectDefinition.RawValue);
	CharacterInstance->GetAttributesComponent()->DecreaseAttribute(CharacterEffectDefinition.TargetedAttribute, CharacterEffectDefinition.RawValue);
}

void UActiveEffectInstance::HandleOvertimeTick()
{
	if (ExecutedOvertimeTicks >= TotalOvertimeTicks)
	{
		GetWorld()->GetTimerManager().ClearTimer(OvertimeTick);
		return;
	}

	ApplyOvertimeTick(CachedOvertimeTickValue);
	ExecutedOvertimeTicks++;
}

void UActiveEffectInstance::Apply()
{
	if (CharacterEffectDefinition.EffectMode == EEffectMode::InstantDuration || 
		CharacterEffectDefinition.EffectMode == EEffectMode::InstantPersistent	
	)
	{
		AppliedRawValue = CharacterEffectDefinition.RawValue;
		CharacterInstance->GetResourceComponent()->UpdateResource(CharacterEffectDefinition.TargetedResource, CharacterEffectDefinition.RawValue);
		CharacterInstance->GetAttributesComponent()->IncreaseAttribute(CharacterEffectDefinition.TargetedAttribute, CharacterEffectDefinition.RawValue);
	}
}

void UActiveEffectInstance::SetTimers()
{
	

	GetWorld()->GetTimerManager().SetTimer(
		DurationTick,
		this,
		&UActiveEffectInstance::RemoveEffect_Timer,
		CharacterEffectDefinition.Duration,
		false
	);

	if (CharacterEffectDefinition.EffectMode != EEffectMode::Overtime)
		return;

	const float TickInterval = 1.0f;

	TotalOvertimeTicks = FMath::FloorToInt(
		CharacterEffectDefinition.Duration / TickInterval
	);

	ExecutedOvertimeTicks = 0;

	CachedOvertimeTickValue =
		CharacterEffectDefinition.RawValue / TotalOvertimeTicks;

	HandleOvertimeTick();
	
	GetWorld()->GetTimerManager().SetTimer(
		OvertimeTick,
		this,
		&UActiveEffectInstance::HandleOvertimeTick,
		TickInterval,
		true
	);
}



void UActiveEffectInstance::RemoveEffect(bool bFailure)
{
	if (bFailure && CharacterEffectDefinition.bRevertOnFailure)
	{
		CharacterInstance->GetResourceComponent()->UpdateResource(CharacterEffectDefinition.TargetedResource, -AppliedRawValue);
		CharacterInstance->GetAttributesComponent()->DecreaseAttribute(CharacterEffectDefinition.TargetedAttribute, -AppliedRawValue);
	}
	
	if (CharacterEffectDefinition.EffectMode == EEffectMode::InstantDuration)
	{
		Revert();
	}
	
	// Clear duration timer if not already cleared
	if (DurationTick.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(DurationTick))
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationTick);
		DurationTick.Invalidate();
	}
	// Clear overtime tick timer if not already cleared
	if (OvertimeTick.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(OvertimeTick))
	{
		GetWorld()->GetTimerManager().ClearTimer(OvertimeTick);
		OvertimeTick.Invalidate();	
	}
	OnEffectEndDelegate.Broadcast(this);
}

void UActiveEffectInstance::RemoveEffect_Timer()
{
	RemoveEffect(false);
}
