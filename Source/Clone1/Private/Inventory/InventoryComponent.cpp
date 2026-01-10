// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include  "Equipment/EquipmentComponent.h"
#include "Structs/FItemData.h"
#include "Inventory/HUD/InventoryWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Algo/Find.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Items.SetNum(Capacity); // Very important - items must be of capacity number
}


void UInventoryComponent::SetEquipmentComponentLink(UEquipmentComponent* inEquipmentComponent)
{
	if (inEquipmentComponent)
	{
		EquipmentComponent = inEquipmentComponent;
	}
}




bool UInventoryComponent::AddItem(FName ItemID, int32 Amount)
{
	if (!HasSpace())
	{
		OnItemRejectedDelegate.Broadcast(ItemID);
		return false;
	};

	// Get requested Item Data from DataTable
	const FItemData* ItemData = GetItemData(ItemID);
	
	if (!ItemData) return false;
	
	// First, try to fill stacks if exists
	for (FItemStack& Item : Items)
	{
		if (Item.ItemID == ItemID && ItemData->ItemBaseData.MaxStack > Item.Amount)
		{
			int32 Space = ItemData->ItemBaseData.MaxStack - Item.Amount;
			int32 ToAdd = FMath::Min(Space, Amount);
			
			Item.Amount += ToAdd;
			Amount -= ToAdd;
			
			
			if (Amount <= 0)
			{
				UpdateWeight();
				return true;
			};
			
		}
	}
	
	// Now, we check if there is any leftover, and still space in the inventory
	if (HasSpace())
	{
		for (FItemStack& Item : Items)
		{
			if (Item.ItemID.IsNone())
			{
				int32 Space = ItemData->ItemBaseData.MaxStack;
				int ToAdd = FMath::Min(Space,Amount);
		
				FItemStack NewItem = FItemStack(ItemID,ToAdd);
				Item.ItemID = NewItem.ItemID;
				Item.Amount = NewItem.Amount;
				Amount -= ToAdd;
				if (Amount <= 0) break;
			}
		}
	}
	
	UpdateWeight();
	OnInventoryChanged.Broadcast();
	return Amount <= 0;
}

void UInventoryComponent::AddItemToIndex(FName ItemID, int32 Amount,int32 InventorySlot)
{
	Items[InventorySlot].ItemID = ItemID;
	Items[InventorySlot].Amount = Amount;
	
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::RemoveItem(int32 Index, int32 Amount, bool bDrop)
{
	// Index cant exceed items
	if (!Items.IsValidIndex(Index) || Amount <= 0) return ;
	
	FItemStack& Stack = Items[Index];
	
	Stack.Amount -= FMath::Min(Stack.Amount, Amount);
	
	if (Stack.Amount <=0)
	{
		FItemStack StackToDrop = Items[Index];
		StackToDrop.Amount = Amount;
		Items[Index].Reset();
		
		if (bDrop)
			OnRequestDropDelegate.Broadcast(StackToDrop);
		
	
	}
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::RemoveItemFromSplit(FName ItemID, int32 Amount)
{ 
	FItemStack BroadcastItem = FItemStack(ItemID, Amount); 
	OnRequestDropDelegate.Broadcast(BroadcastItem);
}

void UInventoryComponent::SplitStack(int32 Index, int32 NewBase)
{
	if (!Items.IsValidIndex(Index)) return;
	
	FItemStack& Stack = Items[Index];
	Stack.Amount = NewBase;
	
	if (Stack.Amount <=0)
	{
		LastRemovedStack = Items[Index];
		Items[Index].Reset();
	}
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}
//Runs on SplitStack abort
void UInventoryComponent::RestoreStack(int32 Index, int32 RestoreAmount)
{
	if (Items[Index].ItemID ==  NAME_None)
	{
		LastRemovedStack.Amount += RestoreAmount;
		Items[Index].ItemID = LastRemovedStack.ItemID;
		Items[Index].Amount = LastRemovedStack.Amount;
	}
	else
	{
		FItemStack& ItemStack = Items[Index];
		ItemStack.Amount += RestoreAmount;
	}

	UpdateWeight();
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::MergeStacks(int32 FromIndex, int32 ToIndex, int32 AppendedAmount)
{
	Items[FromIndex].Amount -= AppendedAmount;
	Items[ToIndex].Amount += AppendedAmount;
	
	if (Items[FromIndex].Amount <=0)
	{
		Items[FromIndex].Reset();
	}
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::SwapStacks(int32 FromIndex, int32 ToIndex)
{	
	FItemStack Temp = Items[FromIndex];
	
	Items[FromIndex] = Items[ToIndex];
	
	Items[ToIndex] = Temp;
	
	OnInventoryChanged.Broadcast();
	
}
//Moving onto a free slot (Inventory <-> Inventory)
void UInventoryComponent::ChangeIndex(int32 FromIndex, int32 ToIndex)
{
	Items[ToIndex] = Items[FromIndex];
	Items[FromIndex] = FItemStack();
	
	OnInventoryChanged.Broadcast();
}

//Artificial Drag -> Inventory Slot
void UInventoryComponent::PopulateIndex(int32 ToIndex, FCarriedStack CarriedStack)
{
	Items[ToIndex].Amount = CarriedStack.Amount;
	Items[ToIndex].ItemID = CarriedStack.ItemID;
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}



// COMMUNICATION Inventory -> Equipment ===============================
//=====================================================================

//  [ ! ] DO NOT CALL THIS FROM EQUIPMENT COMPONENT 
void UInventoryComponent::SwapWithEquipment(int32 InventorySlot)
{
	FItemStack& StackOnSlot = Items[InventorySlot];
	
	// Unequip first
	FName UnequippedID;
	EquipmentComponent->ResponseTryAddItem(StackOnSlot.ItemID,UnequippedID);
	
	
	// Handle returned Item
	// TODO:
	
	
	//Set new ItemID for stack
	StackOnSlot.ItemID = UnequippedID;;
	StackOnSlot.Amount = 1;
	OnInventoryChanged.Broadcast();
}
//  [ ! ] DO NOT CALL THIS FROM EQUIPMENT COMPONENT 
void UInventoryComponent::AddFromEquipment(EEquipmentType EquipmentSlot, int32 InventorySlot, FName EquipmentItemID)
{
	EquipmentComponent->RemoveItem(EquipmentSlot);
	
	Items[InventorySlot].ItemID = EquipmentItemID;
	Items[InventorySlot].Amount = 1;
	
	OnInventoryChanged.Broadcast();
}



// API GETTERS ========================================================
//=====================================================================
const FItemData* UInventoryComponent::GetItemData(FName ItemID) const
{
	return ItemsDataTable->FindRow<FItemData>(ItemID,TEXT(""));
}
const FItemData* UInventoryComponent::GetItemDataByIndex(int32 Index)
{
	FName FoundID = Items[Index].ItemID;
	return ItemsDataTable->FindRow<FItemData>(FoundID,TEXT(""));
}
const TArray<FItemStack>& UInventoryComponent::GetItems()
{
	return Items;
}
const FItemStack* UInventoryComponent::GetStackByIndex(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return &Items[Index];	
	}
	else
	{
		return nullptr;
	}
}



void UInventoryComponent::UpdateWeight()
{
	int32 Weight = 0;
	for (FItemStack Item : Items)
	{
		if (!Item.ItemID.IsNone())
		{
			const FItemData* ItemData = GetItemData(Item.ItemID);
			if (ItemData)
			{
				Weight += (ItemData->ItemBaseData.Weight * Item.Amount);
			}
		}
	}
	
	CurrentWeight = Weight;
	OnWeightChangeDelegate.Broadcast(CurrentWeight,MaxWeight);
}

void UInventoryComponent::AppendAmount(int32 Index, int32 Amount)
{
	Items[Index].Amount += Amount;
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::DecreaseAmount(int32 Index, int32 Amount)
{
	Items[Index].Amount -= Amount;
	
	if (Items[Index].Amount <= 0)
	{
		Items[Index].Reset();
	}
	UpdateWeight();
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::ConsumeItem(int32 Index)
{
	Items[Index].Amount -= 1;
	OnItemConsumedDelegate.Broadcast(Items[Index].ItemID);
	
	if (Items[Index].Amount <= 0)
	{
		Items[Index].Reset();
	}
	UpdateWeight();
	OnInventoryChanged.Broadcast();
	
}
