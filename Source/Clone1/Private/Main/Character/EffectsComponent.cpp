// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/EffectsComponent.h"
#include "Main/Character/AttributesComponent.h"
#include "AbilitySystem/Instances/ActiveEffectInstance.h"
#include "Main/Character/Derived/MainCharacter.h"


// Sets default values for this component's properties
UEffectsComponent::UEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UEffectsComponent::ApplyEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid SourceInstanceID)
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
		
		OnEffectStartDelegate.Broadcast(NewEffect);
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
		OnRequestVFXEndDelegate.Broadcast(EffectInstance->EffectInstanceID);
	
	
	ActiveEffects.Remove(EffectInstance);
	OnEffectEndDelegate.Broadcast(EffectInstance);
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
	for (UActiveEffectInstance* Effect : ActiveEffects)
	{
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
		InventoryComponent->OnItemConsumedDelegate.AddDynamic(this, &UEffectsComponent::ApplyEffect);
	}
}

void UEffectsComponent::SetAbilitySystemComponentLink(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (InAbilitySystemComponent)
	{
		InAbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &UEffectsComponent::OnAbilityAborted);
	}
}
