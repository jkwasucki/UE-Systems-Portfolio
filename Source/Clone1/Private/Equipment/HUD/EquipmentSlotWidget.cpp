// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentSlotWidget.h"
#include "Inventory/HUD/InventorySlotTooltipWidget.h"
#include "Equipment/HUD/EquipmentWidget.h"
#include "Engine/Engine.h"
#include "Inventory/HUD/InventoryDragDropOperation.h"

void UEquipmentSlotWidget::SetData(const FItemData& ItemData)
{
	bIsOccupied = true;
	SetDataBP(ItemData);
	UpdateTooltip();
}

void UEquipmentSlotWidget::Cleanup()
{
	bIsOccupied = false;
	CleanupBP();
	UpdateTooltip();
}

void UEquipmentSlotWidget::ResolveOnDrop(int32 FromSlot, FName droppedItemID)
{
	bool  isEquippable = EquipmentWidget->EquipmentComponent->IsEquippable(droppedItemID);
	bool typeMatches = EquipmentWidget->EquipmentComponent->EquippmentTypeMatch(EquipmentType, droppedItemID);
	
	if (isEquippable && typeMatches)
	{
		if (EquipmentWidget->EquipmentComponent->IsOccupied(EquipmentType))
		{
			OnSwapWithInventoryDelegate.Broadcast(EquipmentType,droppedItemID, FromSlot);
		}
		else
		{
			OnDroppedOnto_FromInventory.Broadcast(EquipmentType,droppedItemID, FromSlot);
		}
		
	}
}

void UEquipmentSlotWidget::UpdateTooltip()
{
	if (!EquipmentWidget)return;
	
	if (!EquipmentWidget->EquipmentComponent->IsOccupied(EquipmentType))
	{
		Button->SetToolTip(nullptr);
		CachedTooltip = nullptr;
		return;
	}
		
		
	if (CachedTooltip == nullptr)
	{
		CachedTooltip = CreateWidget<UInventorySlotTooltipWidget>(GetOwningPlayer(),SlotTooltipWidget);
		if (CachedTooltip)
		{
			Button->SetToolTip(CachedTooltip);
		}
	}
	
	if (!CachedTooltip) return;
	
	const FItemData* ItemData = EquipmentWidget->EquipmentComponent->GetEquipmentDataBySlot(EquipmentType);
	if (!ItemData) return;
				
	CachedTooltip->SetDataBP(ItemData->ItemBaseData.ItemName,ItemData->ItemBaseData.ItemDescription);
	
}

FReply UEquipmentSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                            const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
			OnRightClickDelegate.Broadcast(EquipmentType);
	}
	
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

bool UEquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UInventoryDragDropOperation* DD = Cast<UInventoryDragDropOperation>(InOperation))
	{
		ResolveOnDrop(DD->FromSlotIndex, DD->ItemID);	
		
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
