// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventoryDropAreaWidget.h"

#include "Inventory/HUD/InventoryWidget.h"

FReply UInventoryDropAreaWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                                const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InventoryWidgetRef->bIsCarryingStack)
		{
			int32 Index = InventoryWidgetRef->CarryFromSlotIndex;
			OnDropDelegate.Broadcast(Index,InventoryWidgetRef->CarriedStack.Amount, true);
		}
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}
