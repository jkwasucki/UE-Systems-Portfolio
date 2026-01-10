// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentDragDropOperation.h"

void UEquipmentDragDropOperation::SetDragDropData(EEquipmentType inFromSlot,FName inItemID)
{
	ItemID = inItemID;
	FromSlot = inFromSlot;
}
