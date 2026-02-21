// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/EffectsComponent.h"
#include "Main/Character/AttributesComponent.h"
#include "AbilitySystem/Instances/ActiveEffectInstance.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Net/UnrealNetwork.h"

UEffectsComponent::UEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UEffectsComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEffectsComponent,RepEffects);
}

void UEffectsComponent::OnRep_ActiveEffects()
{
	// New Added:
	for (const FRepActiveEffect& NewEffect : RepEffects)
	{
		bool bFound = CachedEffects.ContainsByPredicate([&](const FRepActiveEffect& Old)
		{
			return Old.EffectInstanceID == NewEffect.EffectInstanceID;
		});
		if (!bFound)
		{
			// If not found in cached, means rep is new
			if (NewEffect.EffectDefinition.bHasVFX)
			{
				OnRequestVFXDelegate.Broadcast(
					NewEffect.EffectDefinition.VFXData,
					NewEffect.EffectInstanceID
				);
				OnEffectStartDelegate.Broadcast(NewEffect.EffectDefinition);
			}
		}
	}
	
	// Removed:
	for (const FRepActiveEffect& OldEffect: CachedEffects)
	{
		bool bStillExists = RepEffects.ContainsByPredicate([&] (const FRepActiveEffect& NewEffect)
		{
			return NewEffect.EffectInstanceID == OldEffect.EffectInstanceID;
		});
		
		if (!bStillExists)
		{
			OnRequestVFXEndDelegate.Broadcast(
				OldEffect.EffectInstanceID
			);
			OnEffectEndDelegate.Broadcast(OldEffect.EffectDefinition, OldEffect.EffectInstanceID);
		}
	}
	CachedEffects = RepEffects;
}

void UEffectsComponent::ApplyEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid SourceInstanceID)
{
	if (!GetOwner()->HasAuthority()) return;
	
	if (IsEffectActiveByTypeID(Effect.TypeID))
	{
		if (!Effect.bCanStack)
		{
			//Reset
		}
		else
		{
			// stack
		}
	}
	else
	{
		UActiveEffectInstance* NewEffect = NewObject<UActiveEffectInstance>(this);
		NewEffect->Initialize(EffectOrigin,GetOwner<ABaseCharacter>(),Effect,SourceInstanceID);
		NewEffect->OnEffectEndDelegate.AddDynamic(this,&UEffectsComponent::HandleEffectEnded);
		
		// EFFECT is applied from some external actor (not this Character)
		if (EffectOrigin != GetOwner())
		{
			if (EffectOrigin->Implements<UCharacterEffectReceiverInterface>())
			{
				UAbilitySystemComponent* ASC = ICharacterEffectReceiverInterface::Execute_GetAbilitySystemComponent(EffectOrigin);
				if (ASC)
					ASC->OnAbilityAbortedDelegate.AddUniqueDynamic(
						this,
						&UEffectsComponent::ExternalOriginAborted
					);
			}
		}
		
		ActiveEffects.Add(NewEffect);
		
		if (NewEffect->CharacterEffectDefinition.bHasVFX)
			OnRequestVFXDelegate.Broadcast(NewEffect->CharacterEffectDefinition.VFXData, NewEffect->EffectInstanceID);
		
			
		OnEffectStartDelegate.Broadcast(NewEffect->CharacterEffectDefinition);
		
		// Replicate
		FRepActiveEffect RepEffect;
		RepEffect.EffectInstanceID = NewEffect->EffectInstanceID;
		RepEffect.SourceInstanceID = NewEffect->SourceInstanceID;
		RepEffect.EffectDefinition = NewEffect->CharacterEffectDefinition;
		RepEffect.StartTime = GetWorld()->GetTimeSeconds();
		RepEffect.Counter++;
		
		RepEffects.Add(RepEffect);
	}
}

void UEffectsComponent::ApplyConsumableEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid SourceInstanceID, FName ItemID)
{
	if (IsEffectActiveByTypeID(Effect.TypeID))
	{
		if (!Effect.bCanStack)
		{
			//REset
		}
		else
		{
			// stack
		}
	}
	else
	{
		UActiveEffectInstance* NewEffect = NewObject<UActiveEffectInstance>(this);
		NewEffect->Initialize(EffectOrigin,GetOwner<ABaseCharacter>(),Effect,SourceInstanceID);
		NewEffect->OnEffectEndDelegate.AddDynamic(this,&UEffectsComponent::HandleEffectEnded);
		
		// EFFECT is applied from some external actor (not this Character)
		if (EffectOrigin != GetOwner())
		{
			if (EffectOrigin->Implements<UCharacterEffectReceiverInterface>())
			{
				UAbilitySystemComponent* ASC = ICharacterEffectReceiverInterface::Execute_GetAbilitySystemComponent(EffectOrigin);
				if (ASC)
					ASC->OnAbilityAbortedDelegate.AddUniqueDynamic(
						this,
						&UEffectsComponent::ExternalOriginAborted
					);
			}
		}
		
		
		ActiveEffects.Add(NewEffect);
		
		if (NewEffect->CharacterEffectDefinition.bHasVFX)
			OnRequestVFXDelegate.Broadcast(NewEffect->CharacterEffectDefinition.VFXData, NewEffect->EffectInstanceID);
	
	
		OnConsumableEffectStartDelegate.Broadcast(NewEffect->CharacterEffectDefinition, ItemID);
		
		// Replicate
		FRepActiveEffect RepEffect;
		RepEffect.EffectInstanceID = NewEffect->EffectInstanceID;
		RepEffect.SourceInstanceID = NewEffect->SourceInstanceID;
		RepEffect.EffectDefinition = NewEffect->CharacterEffectDefinition;
		RepEffect.StartTime = GetWorld()->GetTimeSeconds();
		RepEffect.Counter++;
		
		RepEffects.Add(RepEffect);
	}
}

TArray<FGuid> UEffectsComponent::RemoveEffects(AActor* EffectOrigin, const FGuid& SourceInstanceID, bool bFailure)
{
	TArray<UActiveEffectInstance*> EffectsCopy = ActiveEffects;
	TArray<FGuid> Instances;
	for (UActiveEffectInstance* EffectInstance : EffectsCopy)
	{
		if (EffectInstance && EffectInstance->SourceInstanceID == SourceInstanceID && EffectInstance->EffectOrigin == EffectOrigin)
		{
			Instances.Add(EffectInstance->EffectInstanceID);
			EffectInstance->RemoveEffect(bFailure);
		}
	}
	return Instances;
}

void UEffectsComponent::OnAbilityAborted(AActor* EffectOrigin, const FGuid& SourceInstanceID)
{
	RemoveEffects(EffectOrigin,SourceInstanceID,true);
}

void UEffectsComponent::HandleEffectEnded(UActiveEffectInstance* EffectInstance)
{
	if (!IsValid(EffectInstance))
		return;
	
	if (EffectInstance->CharacterEffectDefinition.bHasVFX)
	{
		OnRequestVFXEndDelegate.Broadcast(EffectInstance->EffectInstanceID);
		// Multicast_OnRequestVFXEnd(EffectInstance->EffectInstanceID);
	}
	
	
	
	ActiveEffects.Remove(EffectInstance);
	OnEffectEndDelegate.Broadcast(EffectInstance->CharacterEffectDefinition, EffectInstance->EffectInstanceID);
	
	// Replicate
	RepEffects.RemoveAllSwap([&](const FRepActiveEffect& E)
	{
		return E.EffectInstanceID == EffectInstance->EffectInstanceID;
	});
	
	
}

void UEffectsComponent::ExternalOriginAborted(AActor* EffectOrigin, const FGuid& SourceInstanceID)
{
	TArray<FGuid> InstanceIDs = RemoveEffects(EffectOrigin,SourceInstanceID,true);
	for (FGuid& InstanceID : InstanceIDs)
	{
		OnRequestVFXEndDelegate.Broadcast(InstanceID);
	}
}

TArray<FCharacterEffect> UEffectsComponent::GetActiveEffectsDefinitions()
{
	TArray<FCharacterEffect> Returns;
	for (UActiveEffectInstance* Instance : ActiveEffects)
	{
		Returns.Add(Instance->CharacterEffectDefinition);		
	}
	return Returns;
}

UActiveEffectInstance* UEffectsComponent::GetEffectByID(FGuid EffectInstanceID) 
{
	for (UActiveEffectInstance* Effect : ActiveEffects)
	{
		if (Effect->EffectInstanceID == EffectInstanceID)
		{
			return Effect;
		}
	}
	return nullptr;
}
float UEffectsComponent::GetDurationForEffect(FGuid EffectInstanceID)
{
	for (UActiveEffectInstance* Effect : ActiveEffects)
	{
		if (Effect->EffectInstanceID == EffectInstanceID)
		{
			return Effect->CharacterEffectDefinition.Duration;
		}
	}
	return 0;
}
TArray<UActiveEffectInstance*> UEffectsComponent::GetConsumableEffects() const
{
	TArray<UActiveEffectInstance*> ConsumableEffects;

	for (int32 i = 0; i < ActiveEffects.Num(); ++i)
	{
		UActiveEffectInstance* Effect = ActiveEffects[i];

		if (!IsValid(Effect))
		{
			UE_LOG(LogTemp, Error,
				TEXT("Invalid effect at index %d in ActiveEffects"), i);
			continue;
		}

		// Extra safety: check definition integrity
		if (Effect->CharacterEffectDefinition.Source == EEffectSource::Consumable)
		{
			ConsumableEffects.Add(Effect);
		}
	}

	return ConsumableEffects;
}


bool UEffectsComponent::IsEffectActiveByInstance(FGuid EffectInstanceID)
{
	for (UActiveEffectInstance* Effect : ActiveEffects)
	{
		if (Effect->EffectInstanceID == EffectInstanceID)
		{
			return true;
		}
	}
	return false;
}

bool UEffectsComponent::IsEffectActiveByTypeID(FName TypeID)
{
	for (UActiveEffectInstance* Effect : ActiveEffects)
	{
		if (Effect->CharacterEffectDefinition.TypeID == TypeID)
		{
			return true;
		}
	}
	return false;
}





void UEffectsComponent::SetAttributesComponentLink(UAttributesComponent* inAttributesComponent)
{
	if (inAttributesComponent)
	{
		AttributesComponent = inAttributesComponent;
	}
}

void UEffectsComponent::SetInventoryComponentLink(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		InventoryComponent->OnItemConsumedDelegate.AddDynamic(this, &UEffectsComponent::ApplyConsumableEffect);
	}
}

void UEffectsComponent::SetAbilitySystemComponentLink(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (InAbilitySystemComponent)
	{
		InAbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &UEffectsComponent::OnAbilityAborted);
	}
}
