// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerState/MainPlayerState.h"

#include "Equipment/EquipmentComponent.h"
#include "Inventory/InventoryComponent.h"


AMainPlayerState::AMainPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryComponent->SetEquipmentComponentLink(EquipmentComponent);
	EquipmentComponent->SetInventoryComponentLink(InventoryComponent);
	
}

TArray<UAbilityData*> AMainPlayerState::GetAbilities() const
{
	return AvailableAbilities;
}

UAbilityData* AMainPlayerState::GetAbilityBySlot(FGameplayTag Tag)
{
	for (const FFAbilitySlot& Slot : AbilitySlots)
	{
		if (Slot.SlotTag == Tag)
			return Slot.AbilityData;
	}
	return nullptr;
}


