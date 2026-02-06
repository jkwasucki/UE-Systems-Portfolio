// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/AbilitySystemComponent.h"

#include "AbilitySystem/Instances/ActiveAbilityInstance.h"
#include "GameplayTagContainer.h"
#include "Interfaces/ResourceInterface.h"
#include "Main/Character/AttributesComponent.h"

// Sets default values for this component's properties
UAbilitySystemComponent::UAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAbilitySystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ActiveTargetingStrategy)
	{
		ActiveTargetingStrategy->TickTargeting(DeltaTime);
	}
}



void UAbilitySystemComponent::TryUseAbility(UAbilityData* Ability)
{
	if (bIsCasting)
		return;
	
	EAbilityFailureReason FailureReason = EAbilityFailureReason::None;
	FAbilityTargetData TargetData;
	if (Validate(Ability, TargetData, FailureReason))
	{
		Commit(Ability);
		Execute(Ability, TargetData);
	}
	else
	{
		OnAbilityCastFailDelegate.Broadcast(Ability,FailureReason);
	}
}

void UAbilitySystemComponent::TryAbortAbility(UAbilityData* Ability)
{
	if (IsAbilityActive(Ability->Tag) && Ability->AbilityCastMode == EAbilityCastMode::Casted)
	{
		OnAbilityAbortedDelegate.Broadcast(GetOwner(), ActiveAbility->AbilityInstanceID);
		ActiveAbility->CancelCast();
		ActiveAbility = nullptr;
		bIsCasting = false;
		OnEndCastDelegate.Broadcast(true);  // Know its stupid
		OnStateRequestDelegate.ExecuteIfBound(EEntityState::Idle);
	}
}

void UAbilitySystemComponent::GrantAbilities(TArray<UAbilityData*> Abilities)
{
	GrantedAbilities = Abilities;
}

bool UAbilitySystemComponent::IsOnCooldown(UAbilityData* Ability)
{
	if (!Ability)
		return false;
	
	FPrimaryAssetId AssetID = Ability->GetPrimaryAssetId();
	if (!AssetID.IsValid())
		return false;
	
	float* EndTime = OngoingCooldowns.Find(AssetID);
	if (!EndTime) 
		return false;
	
	return GetWorld()->GetTimeSeconds() < *EndTime;
}



bool UAbilitySystemComponent::Validate(UAbilityData* Ability, FAbilityTargetData& OutTargetData, EAbilityFailureReason& OutFailReason)
{
	if (!Ability)
		return false;

	
	float AvailableEnergy = IResourceInterface::Execute_GetEnergy(GetOwner());
	if (AvailableEnergy < Ability->EnergyCost)
	{
		OutFailReason = EAbilityFailureReason::InsufficientEnergy;
		return false;
	}

	
	
	if (IsOnCooldown(Ability))
	{
		OutFailReason = EAbilityFailureReason::OnCooldown;
		return false;
	}
	

	if (!GrantedAbilities.Contains(Ability))
		return false;
	
	
	//-- RESOLVE TARGETING (STATE MACHINE)
	if (!ActiveTargetingStrategy ||
		ActiveTargetingStrategy->GetClass() != Ability->TargetingStrategy)
	{
		if (ActiveTargetingStrategy)
		{
			ActiveTargetingStrategy->AbortTargeting();
			ActiveTargetingStrategy = nullptr;
		}

		ActiveTargetingStrategy =
			NewObject<UTargetingStrategy>(this, Ability->TargetingStrategy);
	}
	
	
	 
	EAbilityTargetingStatus Status = ActiveTargetingStrategy->TryResolveTargeting(GetOwner(),Ability->IndicatorData, Ability->CustomTargetingRules, OutTargetData); 
	switch (Status)
	{
		case EAbilityTargetingStatus::Invalid:
		{
			ActiveTargetingStrategy = nullptr;
			OutFailReason = EAbilityFailureReason::InvalidTarget;
			return false;
		}
		case EAbilityTargetingStatus::InProgress:
		{
			OutFailReason = EAbilityFailureReason::TargetingInProgress;		
			return false;
		}
		case EAbilityTargetingStatus::Confirmed:
		{
			ActiveTargetingStrategy = nullptr;
			return true;
		}
	}
	return false;
}

void UAbilitySystemComponent::Commit(UAbilityData* Ability)
{
	
	IResourceInterface::Execute_ModifyEnergy(GetOwner(),-Ability->EnergyCost);
}

void UAbilitySystemComponent::Execute(UAbilityData* Ability,FAbilityTargetData& TargetData)
{
	UActiveAbilityInstance* NewActiveAbilityInstance = NewObject<UActiveAbilityInstance>(this);
	NewActiveAbilityInstance->AbilityTag = Ability->Tag;
	NewActiveAbilityInstance->AbilityInstanceID  = FGuid::NewGuid();
	
	ActiveAbility = NewActiveAbilityInstance;
	ActiveAbility->OnCastFinishedDelegate.AddLambda([this]()
	{
		ActiveAbility = nullptr;
		bIsCasting = false;
		OnStateRequestDelegate.ExecuteIfBound(EEntityState::Idle);
		OnEndCastDelegate.Broadcast(true); // Know its stupid
	});
	
	
	NewActiveAbilityInstance->StartCast(Ability->CastTime);
	bIsCasting = true;
	OnBeginCastDelegate.Broadcast(false);  // Know its stupid
	OnStateRequestDelegate.ExecuteIfBound(EEntityState::Casting);
	
	if (Ability->Effects.Num() == 0)
		return;

	int32 EffectCount = Ability->Effects.Num();
	int32 SuccessCount = 0;
	
	for (TSubclassOf<UAbilityEffect> EffectClass : Ability->Effects)
	{
		if (!EffectClass) continue;
		
		UAbilityEffect* Effect = NewObject<UAbilityEffect>(this, EffectClass);
		
		if (Effect->TryApplyEffect(
			GetOwner(),
			TargetData,
			NewActiveAbilityInstance->AbilityInstanceID
		))
		{
			SuccessCount++;
		}
	}
	if (SuccessCount > 0)
	{
		OnAbilityCastDelegate.Broadcast(Ability,ActiveAbility->AbilityInstanceID,TargetData);
		TrackCooldown(Ability);
	}
	else
	{
		// Return energy cost and abort ability if no effect succeeded
		IResourceInterface::Execute_ModifyEnergy(GetOwner(),Ability->EnergyCost);
		TryAbortAbility(Ability);
		ActiveTargetingStrategy = nullptr;
		OnAbilityCastFailDelegate.Broadcast(Ability,EAbilityFailureReason::InvalidTarget);
	}
		
}

void UAbilitySystemComponent::TrackCooldown(UAbilityData* Ability)
{
	float EndTime = GetWorld()->GetTimeSeconds() +  Ability->Cooldown;
	FPrimaryAssetId AssetID = Ability->GetPrimaryAssetId();
	
	OngoingCooldowns.Add(AssetID, EndTime);
}

bool UAbilitySystemComponent::IsAbilityActive(FGameplayTag AbilityTag) const
{
	if (ActiveAbility == nullptr)
		return false;
	else
		return ActiveAbility->AbilityTag == AbilityTag;
	
	
}
