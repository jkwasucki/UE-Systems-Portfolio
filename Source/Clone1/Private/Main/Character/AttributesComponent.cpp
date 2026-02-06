// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character//AttributesComponent.h"
#include "Main/Character/EffectsComponent.h"
#include "Equipment/EquipmentComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributesComponent::RemoveModifiers(const FItemData& ItemData)
{
	for (TPair<EAttribute, FFAttributeValue> Attribute : ItemData.ItemEquipableData.Modifiers)
	{
		FFAttributeValue* Val = Attributes.Find(Attribute.Key);
		if (Val)
		{
			Val->BonusValue -= Attribute.Value.GetFinal();
		}
	}
	OnAttributesChangedDelegate.Broadcast();
}

void UAttributesComponent::AddModifiers(const FItemData& ItemData)
{
	for (TPair<EAttribute, FFAttributeValue> Attribute : ItemData.ItemEquipableData.Modifiers)
	{
		FFAttributeValue* Val = Attributes.Find(Attribute.Key);
		if (Val)
		{
			Val->BonusValue += Attribute.Value.GetFinal();
		}
	}
	OnAttributesChangedDelegate.Broadcast();
}

void UAttributesComponent::IncreaseAttribute(EAttribute Attribute, float Delta)
{
	if (FFAttributeValue* Value = Attributes.Find(Attribute))
	{
		Value->BonusValue += Delta;
		OnAttributesChangedDelegate.Broadcast();
	}
}

void UAttributesComponent::DecreaseAttribute(EAttribute Attribute, float Delta)
{
	if (FFAttributeValue* Value = Attributes.Find(Attribute))
	{
		Value->BonusValue -= Delta;
		OnAttributesChangedDelegate.Broadcast();
	}
}






float UAttributesComponent::GetFinalAttributeValue(EAttribute Attribute) const
{
	if (const FFAttributeValue* Value = Attributes.Find(Attribute))
	{
		return Value->GetFinal();
	}
	return 0;
}

float UAttributesComponent::GetBaseAttributeValue(EAttribute Attribute) const
{
	if (const FFAttributeValue* Value = Attributes.Find(Attribute))
	{
		return Value->BaseValue;
	}
	return 0;
}

float UAttributesComponent::GetBonusAttributeValue(EAttribute Attribute) const
{
	if (const FFAttributeValue* Value = Attributes.Find(Attribute))
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
	for ( UActiveEffectInstance* Effect : EffectsComponent->GetConsumableEffects())
	{
		if (Effect->CharacterEffectDefinition.TargetedAttribute == Attribute)
		{
			return true;
		}
	}
	return false;
}

const TMap<EAttribute, FFAttributeValue>& UAttributesComponent::GetAttributes()
{
	return Attributes;
}




void UAttributesComponent::SetEquipmentComponentLink(UEquipmentComponent* inEquipmentComponent)
{
	if (inEquipmentComponent)
	{
		inEquipmentComponent->OnItemEquipped2Delegate.AddDynamic(this, &UAttributesComponent::AddModifiers);
		inEquipmentComponent->OnItemUnequipped2Delegate.AddDynamic(this, &UAttributesComponent::RemoveModifiers);
	}
}




