// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerState/MainPlayerState.h"

#include "Equipment/EquipmentComponent.h"
#include "Inventory/InventoryComponent.h"
#include "QuestSystem/QuestComponent.h"


AMainPlayerState::AMainPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	QuestSystemComponent = CreateDefaultSubobject<UQuestComponent>("QuestSystemComponent");
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryComponent->SetEquipmentComponentLink(EquipmentComponent);
	EquipmentComponent->SetInventoryComponentLink(InventoryComponent);
	AbilitySystemComponent->Init(this);
	
	if (HasAuthority())
	{
		AbilitySystemComponent->GrantAbilities(GetAbilities());
	}
	
}


TArray<UAbilityData*> AMainPlayerState::GetAbilities() const
{
	return AvailableAbilities;
}

UAbilityData* AMainPlayerState::GetAbilityBySlot(FGameplayTag SlotTag)
{
	for (const FFAbilitySlot& Slot : AbilitySlots)
	{
		if (Slot.SlotTag == SlotTag)
			return Slot.AbilityData;
	}
	return nullptr;
}


