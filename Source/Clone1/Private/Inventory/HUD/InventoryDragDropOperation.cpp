// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventoryDragDropOperation.h"

void UInventoryDragDropOperation::SetDragDropOperation(int32 inFromSlotIndex, int32 infromAmount, FName inItemID)
{
	FromSlotIndex = inFromSlotIndex;
	FromAmount = infromAmount;
	ItemID = inItemID;
}
