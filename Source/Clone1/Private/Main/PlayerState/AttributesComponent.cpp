// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerState/AttributesComponent.h"

#include "Equipment/EquipmentComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributesComponent::RemoveModifiers(const FItemData& ItemData)
{
	for (TPair<EAttribute, FAttributeValue> Attribute : ItemData.ItemEquipableData.Modifiers)
	{
		FAttributeValue* Val = Attributes.Find(Attribute.Key);
		if (Val)
		{
			Val->BonusValue -= Attribute.Value.GetFinal();
		}
	}
	OnAttributesChangedDelegate.Broadcast();
}

void UAttributesComponent::AddModifiers(const FItemData& ItemData)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green,TEXT("Adding Modifiers"));
	for (TPair<EAttribute, FAttributeValue> Attribute : ItemData.ItemEquipableData.Modifiers)
	{
		FAttributeValue* Val = Attributes.Find(Attribute.Key);
		if (Val)
		{
			Val->BonusValue += Attribute.Value.GetFinal();
		}
	}
	OnAttributesChangedDelegate.Broadcast();
}

float UAttributesComponent::GetFinalAttributeValue(EAttribute Attribute) const
{
	if (const FAttributeValue* Value = Attributes.Find(Attribute))
	{
		return Value->GetFinal();
	}
	return 0;
}

float UAttributesComponent::GetBaseAttributeValue(EAttribute Attribute) const
{
	if (const FAttributeValue* Value = Attributes.Find(Attribute))
	{
		return Value->BaseValue;
	}
	return 0;
}

float UAttributesComponent::GetBonusAttributeValue(EAttribute Attribute) const
{
	if (const FAttributeValue* Value = Attributes.Find(Attribute))
	{
		return Value->BonusValue;
	}
	return 0;
}

const FItemBaseData& UAttributesComponent::GetBaseItemData(FName ItemID)
{
	return ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""))->ItemBaseData;
}

bool UAttributesComponent::IsAnyBuffActiveForAttribute(EAttribute Attribute)
{
	for ( TPair<FName, FConsumableEffect> Pair : ConsumableEffects)
	{
		if (Pair.Value.Attribute == Attribute)
		{
			return true;
		}
	}
	return false;
}

float UAttributesComponent::GetDurationForEffect(FName ItemID)
{
	if (FConsumableEffect* Effect = ConsumableEffects.Find(ItemID))
	{
		return Effect->Duration;
	}
	return 0;
}

void UAttributesComponent::OnConsumableUsed(FName ItemID)
{
	const FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""));
	if (ItemData && ItemData->bIsConsumable && ItemData->ItemConsumableData.ConsumableType == EConsumableType::Potion)
	{
		if (FTimerHandle* Timer = EffectTimers.Find(ItemID))
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(*Timer))
			{
				ResetConsumableEffect(ItemID,ItemData->ItemConsumableData.ConsumableEffect);
				return;
			}
		}
		
		ApplyConsumableEffect(ItemID,ItemData->ItemConsumableData.ConsumableEffect);
		
	}
	OnConsumableEffectAppearDelegate.Broadcast();
	OnAttributesChangedDelegate.Broadcast();
}

void UAttributesComponent::ApplyConsumableEffect(FName ItemID, const FConsumableEffect& Effect)
{
	if (FAttributeValue* Value = Attributes.Find(Effect.Attribute))
	{
		ConsumableEffects.Add(ItemID,  Effect);
		
		Value->BonusValue += Effect.Value;
		
		FTimerHandle& TimerHandle = EffectTimers.Add(ItemID);
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this,&UAttributesComponent::RemoveConsumableEffect,ItemID);
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			TimerDelegate,
			Effect.Duration,
			false
		);
		
	}

}

void UAttributesComponent::ResetConsumableEffect(FName ItemID, const FConsumableEffect& Effect)
{
	if (FTimerHandle* TimerHandle = EffectTimers.Find(ItemID))
	{
		float remaining = GetWorld()->GetTimerManager().GetTimerRemaining(*TimerHandle);
		
		float newTime = FMath::Max(remaining, Effect.Duration);
		UE_LOG(LogTemp,Warning, TEXT("NEW TIME: %f"), newTime);
		GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
		
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this,&UAttributesComponent::RemoveConsumableEffect,ItemID);
		
		GetWorld()->GetTimerManager().SetTimer(*TimerHandle,TimerDelegate, newTime, false);
		OnConsumableEffectAppearDelegate.Broadcast();
		OnAttributesChangedDelegate.Broadcast();
	}
	
}

void UAttributesComponent::RemoveConsumableEffect(FName ItemID)
{
	
	if (const FConsumableEffect* Effect = ConsumableEffects.Find(ItemID))
	{
		if (FAttributeValue* Value = Attributes.Find(Effect->Attribute))
		{
			Value->BonusValue -= Effect->Value;
		}
		
		ConsumableEffects.Remove(ItemID);
		EffectTimers.Remove(ItemID);
		
		OnConsumableEffectEndDelegate.Broadcast(ItemID);
		OnAttributesChangedDelegate.Broadcast();
	}
}

const TMap<FName, FConsumableEffect>& UAttributesComponent::GetConsumableEffects()
{
	return ConsumableEffects;
}


void UAttributesComponent::SetEquipmentComponentLink(UEquipmentComponent* inEquipmentComponent)
{
	if (inEquipmentComponent)
	{
		inEquipmentComponent->OnItemEquipped2Delegate.AddDynamic(this, &UAttributesComponent::AddModifiers);
		inEquipmentComponent->OnItemUnequipped2Delegate.AddDynamic(this, &UAttributesComponent::RemoveModifiers);
	}
}

void UAttributesComponent::SetInventoryComponentLink(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		InventoryComponent->OnItemConsumedDelegate.AddDynamic(this, &UAttributesComponent::OnConsumableUsed);
	}
}

