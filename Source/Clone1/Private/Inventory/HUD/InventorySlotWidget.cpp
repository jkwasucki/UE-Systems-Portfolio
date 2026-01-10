// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventorySlotWidget.h"

#include "Equipment/HUD/EquipmentDragDropOperation.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/HUD/InventorySlotTooltipWidget.h"
#include "Inventory/HUD/InventoryDragDropOperation.h"

// WHEN DROPPING ITEM ONTO THIS SLOT AFTER SPLIT (Artificial CarriedStack payload)
FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton &&
		InventoryWidget &&
		InventoryWidget->bIsCarryingStack)
	{
		ResolveDropOnto(
			-1,
			InventoryWidget->CarriedStack.ItemID,
			InventoryWidget->CarriedStack.Amount,
			/*bFromSplit=*/ true
		);

		return FReply::Handled(); // stop drag from starting
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (IsOccupied())
		{
			
			OnRightClickDelegate.Broadcast(SlotIndex);
		}
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

// WHEN DROPPING SOME ITEM ONTO THIS SLOT (Drag and Drop Operation)
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                        UDragDropOperation* InOperation)
{
	
	// Cast to correct DragDrop Operation
	if (UInventoryDragDropOperation* DragDropOP_INV = Cast<UInventoryDragDropOperation>(InOperation))
	{
		ResolveDropOnto(DragDropOP_INV->FromSlotIndex, DragDropOP_INV->ItemID, DragDropOP_INV->FromAmount,false);
	}
	// Cast to correct DragDrop Operation
	if (UEquipmentDragDropOperation* DragDropOP_EQ = Cast<UEquipmentDragDropOperation>(InOperation))
	{
		ResolveDropOnto_FromEquipment(DragDropOP_EQ->FromSlot,DragDropOP_EQ->ItemID);
	}
    	
	
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	ContentBox->SetVisibility(ESlateVisibility::Collapsed);
}



void UInventorySlotWidget::SetData(const FItemStack& ItemStack, const FItemData& ItemData)
{
	SetDataBP(ItemStack, ItemData);
	UpdateTooltip();
}
void UInventorySlotWidget::Cleanup()
{
	CleanupBP();
	UpdateTooltip();
}




// DROP RESOLVE
void UInventorySlotWidget::ResolveDropOnto_FromEquipment(EEquipmentType FromSlot, FName& DroppedItemID)
{
	if (!IsOccupied())
	{
		OnMoveOntoFromEquipmentDelegate.Broadcast(FromSlot,SlotIndex,DroppedItemID);
	}
	else
	{
		FName thisItemID = InventoryWidget->GetItemID(SlotIndex);
		const FItemData* thisItemData = InventoryWidget->InventoryComponent->GetItemData(thisItemID);
		
		if (thisItemData)
		{
			// If this slot holds the same Equipment Type item, we can try swap
			if (thisItemData->bIsEquipable && thisItemData->ItemEquipableData.EquipmentType == FromSlot)
			{
				OnSwapEquipmentItemsDelegate.Broadcast(SlotIndex);
			}
		}
	}
}
void UInventorySlotWidget::ResolveDropOnto(int32 FromSlot, FName ItemID, int32 Amount, bool bFromSplit)
{
	
	if (IsOccupied())
	{
		//Get self item data
		FName ThisItemID = InventoryWidget->GetItemID(SlotIndex);

		//If items are the same
		if (ThisItemID == ItemID)
		{
			int32 RequestedAmount = Amount;
			int32 Space =  InventoryWidget->GetMaxStack(SlotIndex) - InventoryWidget->GetStackAmount(SlotIndex);
			int32 ToAdd = FMath::Min(Space, RequestedAmount);
			// If we have space, merge stacks
			if (ToAdd > 0)
			{
				
				if (bFromSplit)
				{
					OnMergeStacksFromSplitDelegate.Broadcast(SlotIndex,InventoryWidget->CarriedStack.Amount);
				}
				else
				{
					OnMergeStacksDelegate.Broadcast(FromSlot,SlotIndex, ToAdd);
				}
				return;
			}
		}
		
		if (bFromSplit)
		{
			OnSwapFromSplitDelegate.Broadcast(SlotIndex);
		}
		else
		{
			OnSwapDelegate.Broadcast(FromSlot,SlotIndex);
		}
	}
	else
	{
		
		OnMoveOntoDelegate.Broadcast(FromSlot,SlotIndex,bFromSplit);
	}
}

// TOOLTIP
void UInventorySlotWidget::UpdateTooltip()
{
	
	if (!IsOccupied())
	{
		Button->SetToolTip(nullptr);
		CachedTooltip = nullptr;
		return;
	}
	
	if (!CachedTooltip && SlotTooltipWidget)
	{
		CachedTooltip = CreateWidget<UInventorySlotTooltipWidget>(GetOwningPlayer(),SlotTooltipWidget);
		
	}
	if (CachedTooltip)
	{
		CachedTooltip->SetDataBP(InventoryWidget->GetItemName(SlotIndex),InventoryWidget->GetItemDescription(SlotIndex));
		Button->SetToolTip(CachedTooltip);
	}
}



