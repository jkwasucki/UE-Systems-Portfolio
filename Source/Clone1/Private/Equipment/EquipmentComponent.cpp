// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentComponent.h"
#include "Engine/Engine.h"
#include "Inventory/InventoryComponent.h"
// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

}



bool UEquipmentComponent::ResponseTryAddItem(FName ItemID, FName& OutReturnedEquippedItem)
{
	const FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""));
	if (!ItemData) return false;
	if (!ItemData->bIsEquipable) return false;
	
	EEquipmentType Type = ItemData->ItemEquipableData.EquipmentType;
	
	if (Equipment.Contains(Type))
	{
		OutReturnedEquippedItem = Equipment[Type];
		Equipment[Type] = ItemID;
		OnEquipmentChanged.Broadcast();
		OnItemEquippedDelegate.Broadcast(ItemData->ItemEquipableData.VisualMesh,ItemData->ItemEquipableData.EquipmentType);
		OnItemEquipped2Delegate.Broadcast(*ItemData);
		return true;
	}
	else
	{
		OutReturnedEquippedItem = NAME_None;
		
		Equipment.Add(Type, ItemID);
		OnEquipmentChanged.Broadcast();
		OnItemEquippedDelegate.Broadcast(ItemData->ItemEquipableData.VisualMesh,ItemData->ItemEquipableData.EquipmentType);
		OnItemEquipped2Delegate.Broadcast(*ItemData);
		return true;
	}
	
	
}

void UEquipmentComponent::RemoveItem(EEquipmentType Slot)
{
	if (Equipment.Contains(Slot))
	{
		FName ItemID = Equipment[Slot];
		const FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""));

		//Can unequip
		Equipment.Remove(Slot);
		OnEquipmentChanged.Broadcast();
		OnItemUnequippedDelegate.Broadcast(ItemData->ItemEquipableData.EquipmentType);
		OnItemUnequipped2Delegate.Broadcast(*ItemData);
	}
}

void UEquipmentComponent::SwapWithInventory(EEquipmentType Slot, FName DroppedItemID,  int32 InventorySlot)
{
	// Unequip previous mesh and store itemID on equipment slot 
	FName TempItemID = Equipment[Slot];
	const FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(TempItemID, TEXT(""));
	OnItemUnequippedDelegate.Broadcast(ItemData->ItemEquipableData.EquipmentType);
	OnItemUnequipped2Delegate.Broadcast(*ItemData);
	
	// Assign new ItemID to equipment slot and Add to inventory our stored itemID (swap)
	Equipment[Slot] = DroppedItemID;
	InventoryComponent->AddItemToIndex(TempItemID,1, InventorySlot);
	OnEquipmentChanged.Broadcast();
	
	//Equip new mesh
	const FItemData* newItemData = ItemsDataTable->FindRow<FItemData>(DroppedItemID, TEXT(""));
	OnItemEquippedDelegate.Broadcast(newItemData->ItemEquipableData.VisualMesh,newItemData->ItemEquipableData.EquipmentType);
	OnItemEquipped2Delegate.Broadcast(*ItemData);
}

void UEquipmentComponent::AddFromInventory(EEquipmentType EquipmentSlot, FName EquipmentItemID, int32 InventorySlot)
{
	Equipment.Add(EquipmentSlot, EquipmentItemID);
	
	InventoryComponent->RemoveItem(InventorySlot,1,false);
	
	
	OnEquipmentChanged.Broadcast();
	//Equip new mesh
	const FItemData* newItemData = ItemsDataTable->FindRow<FItemData>(EquipmentItemID, TEXT(""));
	OnItemEquippedDelegate.Broadcast(newItemData->ItemEquipableData.VisualMesh,newItemData->ItemEquipableData.EquipmentType);
	OnItemEquipped2Delegate.Broadcast(*newItemData);
}


// Moves Item to the first free slot inside Inventory (when: on Equipment Slot right-press)
void UEquipmentComponent::MoveToInventory(EEquipmentType ItemOnSlot)
{
	FName ItemID = Equipment[ItemOnSlot];
	
	Equipment.Remove(ItemOnSlot);
	InventoryComponent->AddItem(ItemID, 1);
	
	
	OnEquipmentChanged.Broadcast();
	const FItemData* newItemData = ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""));
	OnItemUnequippedDelegate.Broadcast(newItemData->ItemEquipableData.EquipmentType);
	OnItemUnequipped2Delegate.Broadcast(*newItemData);
}

bool UEquipmentComponent::IsEquippable(FName ItemID)
{
	return ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""))->bIsEquipable;
}

bool UEquipmentComponent::EquippmentTypeMatch(EEquipmentType EquipmentType, FName ItemID)
{
	return ItemsDataTable->FindRow<FItemData>(ItemID, TEXT(""))->ItemEquipableData.EquipmentType ==  EquipmentType;
}

const FItemData* UEquipmentComponent::GetEquipmentDataBySlot(EEquipmentType Slot)
{
	if (Equipment.Contains(Slot))
	{
		return ItemsDataTable->FindRow<FItemData>(Equipment[Slot], TEXT(""));
	}
	else
	{
		return nullptr;
	}
}

bool UEquipmentComponent::IsOccupied(EEquipmentType Slot)
{
	return Equipment.Contains(Slot);
}

FName UEquipmentComponent::GetItemID(EEquipmentType Slot)
{
	if (Equipment.Contains(Slot))
	{
		return Equipment[Slot];
	}
	
	return NAME_None;
	
}

UTexture2D* UEquipmentComponent::GetItemTexture(EEquipmentType Slot)
{
	if (Equipment.Contains(Slot))
	{
		return ItemsDataTable->FindRow<FItemData>(Equipment[Slot], TEXT(""))->ItemBaseData.ItemIcon;
	}
	return nullptr;
}

void UEquipmentComponent::SetInventoryComponentLink(UInventoryComponent* inInventoryComponent)
{
	if (inInventoryComponent)
	{
		InventoryComponent = inInventoryComponent;
	}
}






