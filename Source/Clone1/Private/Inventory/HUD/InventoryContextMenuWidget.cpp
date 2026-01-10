// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventoryContextMenuWidget.h"

#include "Components/Button.h"

void UInventoryContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetupButtons();
}

void UInventoryContextMenuWidget::Toggle(bool bState, bool bCanEquip, bool bCanSplit, bool bCanConsume, int32 SlotIndex)
{
	bIsActive = bState;
	
	if (bState)
	{
		ContextSlotIndex = SlotIndex;
		if (bCanConsume)
			ToggleButton(true, EInventoryContextAction::Consume);
		else 
			ToggleButton(false, EInventoryContextAction::Consume);
		if (bCanSplit)
			ToggleButton(true,EInventoryContextAction::Split);
		else
			ToggleButton(false, EInventoryContextAction::Split);
		if (bCanEquip)
			ToggleButton(true,EInventoryContextAction::Equip);
		else
			ToggleButton(false, EInventoryContextAction::Equip);
	}
	else
	{
		ContextSlotIndex = INDEX_NONE;
		ToggleButton(false,EInventoryContextAction::Consume);
		ToggleButton(false,EInventoryContextAction::Split);
		ToggleButton(false,EInventoryContextAction::Equip);
	}
}

void UInventoryContextMenuWidget::ToggleButton(bool bState, EInventoryContextAction ContextAction)
{
	switch (ContextAction)
	{
		case EInventoryContextAction::Consume:
			ConsumeBox->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			break;
		case EInventoryContextAction::Equip:
			EquipBox->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			break;
		case EInventoryContextAction::Split:
			SplitBox->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			break;
	}
}

void UInventoryContextMenuWidget::SetupButtons()
{
	UWidget* EquipButtonChild = EquipBox->GetChildAt(0);
	UButton* EquipButton = Cast<UButton>(EquipButtonChild);
	
	UWidget* SplitButtonChild = SplitBox->GetChildAt(0);
	UButton* SplitButton = Cast<UButton>(SplitButtonChild);
	
	UWidget* ConsumeButtonChild = ConsumeBox->GetChildAt(0);
	UButton* ConsumeButton = Cast<UButton>(ConsumeButtonChild);
	
	
	EquipButton->OnClicked.AddDynamic(this,&UInventoryContextMenuWidget::EquipPress);
	SplitButton->OnClicked.AddDynamic(this,&UInventoryContextMenuWidget::SplitPress);
	ConsumeButton->OnClicked.AddDynamic(this, &UInventoryContextMenuWidget::ConsumePress);
}


void UInventoryContextMenuWidget::EquipPress()
{
	OnEquipDelegate.Broadcast(ContextSlotIndex);
}

void UInventoryContextMenuWidget::SplitPress()
{
	OnSplitDelegate.Broadcast(ContextSlotIndex);
}

void UInventoryContextMenuWidget::ConsumePress()
{
	OnConsumeDelegate.Broadcast(ContextSlotIndex);
}
