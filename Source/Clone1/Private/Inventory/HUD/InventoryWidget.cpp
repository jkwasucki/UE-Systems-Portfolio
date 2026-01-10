// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventoryWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Inventory/HUD/InventorySlotWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/HUD/InventoryDropAreaWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Inventory/HUD/InventorySlotDraggedWidget.h"
#include "Main/PlayerState/MainPlayerState.h"




// ONE-TIME Initialization
void UInventoryWidget::Init(UInventoryScreenWidget* inOrchestrator)
{
	if (bIsInitialized) return;
	
	bIsInitialized = true;
	
	if (inOrchestrator && inOrchestrator->PC)
	{
		Orchestrator = inOrchestrator; 
		if (AMainPlayerState* PS = inOrchestrator->PC->GetPlayerState<AMainPlayerState>())
		{
			if (PS)
			{
				if (PS->InventoryComponent)
				{
					//Inventory Widget listens to its source of truth - InventoryComponent
					InventoryComponent = PS->InventoryComponent;
					PS->InventoryComponent->OnInventoryChanged.AddDynamic(this,&UInventoryWidget::UpdateSlots);
					inOrchestrator->OnRightClickDelegate.AddDynamic(this,&UInventoryWidget::HandleRightClickGLOBAL);
					inOrchestrator->OnModalsDisabledDelegate.AddDynamic(this, &UInventoryWidget::OnModalsDisabled);
					
					// InventoryWidget binds events to InventoryComponent (Component does not know about Widget)
					OnItemDroppedDelegate.AddDynamic(InventoryComponent,&UInventoryComponent::RemoveItem);
					OnItemSplitDroppedDelegate.AddDynamic(InventoryComponent,&UInventoryComponent::RemoveItemFromSplit);
					OnStackSplitDelegate.AddDynamic(InventoryComponent,&UInventoryComponent::SplitStack);
					OnAbortSplitDragDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::RestoreStack);
					OnMergeStacksForwardDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::MergeStacks);
					OnSwapForwardDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::SwapStacks);
					OnChangeIndexDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::ChangeIndex);
					OnPopulateIndexDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::PopulateIndex);
					OnRequestEquipDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::SwapWithEquipment);
					OnMergeStacksFromSplitDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::AppendAmount);
					OnRequestDecreaseAmountDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::DecreaseAmount);
					OnRequestConsumeDelegate.AddDynamic(InventoryComponent,&UInventoryComponent::ConsumeItem);
				}
			}
		}
		
	}
	
	GenerateSlots();
	
	//Subscribe subcomponents of this widget
	if (BP_DropAreaWidget)
	{
		BP_DropAreaWidget->OnDropDelegate.AddDynamic(this, &UInventoryWidget::NotifyItemDropped);
		UInventoryDropAreaWidget* DropAreaWidget = Cast<UInventoryDropAreaWidget>(BP_DropAreaWidget);
		if (DropAreaWidget)
		{
			DropAreaWidget->InventoryWidgetRef = this;
		}
	}
}



void UInventoryWidget::Enable()
{
	SetVisibility(ESlateVisibility::Visible);
	UpdateSlots();
}
void UInventoryWidget::Disable()
{
	if (bIsCarryingStack)
	{
		EndCarry(false);
	}
	if (IsSplitting)
	{
		StopSplitting();
	}
		
	//Kill drag and drop operation
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().CancelDragDrop();
	}
}



// INPUT RESPONSES =====================================
//======================================================
void UInventoryWidget::HandleRightClickGLOBAL()
{
	if (bIsCarryingStack)
	{
		EndCarry(false);
	}
}





// ARTIFICIAL DRAG (CARRY) =====================================
//==============================================================
void UInventoryWidget::BeginCarry(const FItemStack& InStack)
{
	
	const FItemData* Data = InventoryComponent
	? InventoryComponent->GetItemData(InStack.ItemID)
	: nullptr;
	
	CarriedStack = {InStack.ItemID, InStack.Amount};
	bIsCarryingStack = true;
	OnRequestScreenWidget_DragWidget.Broadcast(true);
	Orchestrator->DraggedWidgetInstance->SetDragVisuals(Data->ItemBaseData.ItemIcon, InStack.Amount);
}
void UInventoryWidget::BeginCarryFromSplit(int32 NewStackAmount, int32 Index)
{
	OnRequestScreenWidget_DragWidget.Broadcast(true);
	OnRequestScreenWidget_SplitWidget.Broadcast(false);
	
	CarryFromSlotIndex = CurrentSplittingSlotIndex != INDEX_NONE ? CurrentSplittingSlotIndex : Index;
	StopSplitting(false);
	CurrentSplittingSlotIndex = INDEX_NONE;
	//Get relevant data from source
	UTexture2D* Icon = GetItemIcon(CarryFromSlotIndex);
	const FItemStack* Stack = InventoryComponent->GetStackByIndex(CarryFromSlotIndex);
	FName ItemID = Stack->ItemID;
	
	//Notify about new base amount for the stack on slot
	int32 CurrentStackAmount = Stack->Amount;


	int32 StackNewBase = CurrentStackAmount - NewStackAmount;
	OnStackSplitDelegate.Broadcast(CarryFromSlotIndex, StackNewBase);
	
	CarriedStack = {ItemID, NewStackAmount};
	
	bIsCarryingStack = true;
	
	
	Orchestrator->DraggedWidgetInstance->SetDragVisuals(Icon, NewStackAmount);
}
void UInventoryWidget::EndCarry(bool IsSuccess)
{
	OnRequestScreenWidget_DragWidget.Broadcast(false);
	
	if (!IsSuccess)
	{
		OnAbortSplitDragDelegate.Broadcast(CarryFromSlotIndex, CarriedStack.Amount);
	}
	
	CarryFromSlotIndex = INDEX_NONE;
	CarriedStack.Reset();
	bIsCarryingStack = false;
}


// API =======================================================
//============================================================
bool UInventoryWidget::IsSlotOccupied(int32 SlotIndex)
{
	return !InventoryComponent->Items[SlotIndex].ItemID.IsNone();
}
UTexture2D* UInventoryWidget::GetItemIcon(int32 SlotIndex)
{
	if (const FItemStack* ItemStack = InventoryComponent->GetStackByIndex(SlotIndex))
	{
		if (ItemStack)
		{
			FName ItemID = ItemStack->ItemID;
			const FItemData* ItemData = InventoryComponent->GetItemData(ItemID);
			if (ItemData)
			{
				
				return ItemData->ItemBaseData.ItemIcon;

			}
			return nullptr;
		}
	}
	return nullptr;
}
const int32 UInventoryWidget::GetStackAmount(int32 SlotIndex)
{
	if (const FItemStack* ItemStack = InventoryComponent->GetStackByIndex(SlotIndex))
	{
		return ItemStack->Amount;
	}
	return 0;
}
FName UInventoryWidget::GetItemID(int32 SlotIndex)
{
	return InventoryComponent->GetStackByIndex(SlotIndex)->ItemID;
}
int32 UInventoryWidget::GetMaxStack(int32 SlotIndex)
{
	FName ItemID = InventoryComponent->GetStackByIndex(SlotIndex)->ItemID;
	const FItemData* ItemData = InventoryComponent->GetItemData(ItemID);
	if (ItemData)
	{
		return ItemData->ItemBaseData.MaxStack;
	}
	return 0;
}
FText UInventoryWidget::GetItemName(int32 SlotIndex)
{
	const FItemStack* Stack = InventoryComponent->GetStackByIndex(SlotIndex);
	if (Stack)
	{
		const FItemData* ItemData = InventoryComponent->GetItemData(Stack->ItemID);
		if (ItemData)
		{
			return ItemData->ItemBaseData.ItemName;
		}
	}
	return FText();
}
FText UInventoryWidget::GetItemDescription(int32 SlotIndex)
{
	const FItemStack* Stack = InventoryComponent->GetStackByIndex(SlotIndex);
	if (Stack)
	{
		const FItemData* ItemData = InventoryComponent->GetItemData(Stack->ItemID);
		if (ItemData)
		{
			return ItemData->ItemBaseData.ItemDescription;
		}
	}
	return FText();
}


// MERGE =======================================================
//==============================================================
void UInventoryWidget::RequestMergeStacks(int32 FromIndex, int32 ToIndex, int32 AppendedAmount)
{
	OnMergeStacksForwardDelegate.Broadcast(FromIndex,ToIndex,AppendedAmount);
}
void UInventoryWidget::RequestMergeStacksFromSplit(int32 ToIndex, int32 Amount)
{
	OnMergeStacksFromSplitDelegate.Broadcast(ToIndex,Amount);
	EndCarry(true);
}

// SWAP =======================================================
//=============================================================
void UInventoryWidget::RequestSwap(int32 FromIndex, int32 ToIndex)
{
	
	OnSwapForwardDelegate.Broadcast(FromIndex,ToIndex);
}
void UInventoryWidget::RequestSwapFromSplit(int ToIndex)
{
	FItemStack StackClone = *InventoryComponent->GetStackByIndex(ToIndex);
	OnPopulateIndexDelegate.Broadcast(ToIndex,CarriedStack);
	EndCarry(true);
	BeginCarry(StackClone);
}



// OTHERS =====================================
//=============================================
void UInventoryWidget::OnModalsDisabled()
{
	if (bIsCarryingStack)
	{
		EndCarry(false);
	}
	if (bIsContextMenuOpen)
	{
		OnContextMenuDisabled();
	}
	if (IsSplitting)
	{
		StopSplitting();
	}
}
void UInventoryWidget::UpdateWeight()
{
	WeightText->SetText(FText::Format(
		FText::FromString("{0}/{1}"),
		InventoryComponent->CurrentWeight,
		InventoryComponent->MaxWeight
	));
	
	if (InventoryComponent->CurrentWeight > InventoryComponent->MaxWeight)
	{
		WeightText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
	else
	{
		WeightText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}
void UInventoryWidget::NotifyItemDropped(int32 SlotIndex, int32 Amount, bool bFromSplit)
{
	if (bFromSplit)
	{
		FName carriedItemID = CarriedStack.ItemID;
		int32 carriedAmount = CarriedStack.Amount;
		OnItemSplitDroppedDelegate.Broadcast(carriedItemID,carriedAmount);
		EndCarry(true);
	}
	else
	{
		OnItemDroppedDelegate.Broadcast(SlotIndex,Amount, true);
	}
	
}
void UInventoryWidget::RequestChangeIndex(int32 FromIndex, int32 ToIndex,bool bFromSplit)
{
	if (bFromSplit)
	{
		OnPopulateIndexDelegate.Broadcast(ToIndex, CarriedStack);
		EndCarry(true);
	}
	else
	{
		OnChangeIndexDelegate.Broadcast(FromIndex,ToIndex);
	}
}
void UInventoryWidget::ContextMenu_RequestEquip(int32 SlotIndex)
{
	OnRequestScreenWidget_ContextWidget.Broadcast(false);
	
	OnRequestEquipDelegate.Broadcast(SlotIndex);
}
void UInventoryWidget::HandleRightClickOnSlot(int32 SlotIndex)
{
	const FItemData* ItemData =InventoryComponent->GetItemDataByIndex(SlotIndex);
	
	bool canSplit = InventoryComponent->GetStackByIndex(SlotIndex)->Amount > 1;
	UE_LOG(LogTemp,Warning,TEXT("Amount %d"), InventoryComponent->GetStackByIndex(SlotIndex)->Amount)
	bool isEquipable = ItemData->bIsEquipable;
	bool isConsumable = ItemData->bIsConsumable;
	
	if (canSplit || isEquipable || isConsumable)
	{
		RequestContextMenu(isEquipable,canSplit,isConsumable, SlotIndex);
	}
	
}



// SPLIT WIDGET============================================
//=========================================================
void UInventoryWidget::StartSplitting(int32 SlotIndex)
{
	if (SlotIndex == INDEX_NONE)return;	
	
	//In case it was open
	OnRequestScreenWidget_ContextWidget.Broadcast(false);
	// Request widget from parent
	OnRequestScreenWidget_SplitWidget.Broadcast(true);
	
	if (const FItemStack* ItemStack = InventoryComponent->GetStackByIndex(SlotIndex))
	{
		if (ItemStack->Amount > 1)
		{
			
			IsSplitting = true;
			CurrentSplittingSlotIndex = SlotIndex;
			
	
			FName ItemID = ItemStack->ItemID;
			const FItemData* ItemData = InventoryComponent->GetItemData(ItemID);
			if (ItemData)
			{
				int32 Amount = ItemStack->Amount;
				UTexture2D* Icon = ItemData->ItemBaseData.ItemIcon;

				Orchestrator->InventorySplitWidget->SetData(Amount, Icon);
			}
			
		}
	}
	
	
}
void UInventoryWidget::StopSplitting(bool bAbort)
{
	OnRequestScreenWidget_SplitWidget.Broadcast(false);
	
	IsSplitting = false;
	if (bAbort)
	{
		CurrentSplittingSlotIndex = INDEX_NONE;
	}
}


// REFERENCE TO WIDGETS FROM ORCHESTRATOR ======================
//==============================================================
void UInventoryWidget::SetupContextMenu(UInventoryContextMenuWidget* ContextMenuWidget)
{
	ContextMenuWidget->OnConsumeDelegate.AddDynamic(this, &UInventoryWidget::Consume);
	ContextMenuWidget->OnEquipDelegate.AddDynamic(this,&UInventoryWidget::ContextMenu_RequestEquip);
	ContextMenuWidget->OnSplitDelegate.AddDynamic(this, &UInventoryWidget::StartSplitting);

}
void UInventoryWidget::SetupSplitWidget(UInventorySlotSplitWidget* SplitWidget)
{
	if (SplitWidget)
	{
		SplitWidget->OnDismissDelegate.AddDynamic(this, &UInventoryWidget::StopSplitting);
		SplitWidget->OnConfirmDelegate.AddDynamic(this, &UInventoryWidget::BeginCarryFromSplit);
	}
}


// CONTEXT WIDGET==========================================
//=========================================================
void UInventoryWidget::RequestContextMenu(bool bIsEquippable, bool bIsSplittable, bool bIsConsumable, int32 SlotIndex)
{
	OnRequestScreenWidget_ContextWidget.Broadcast(true);
	
	Orchestrator->InventoryContextMenuInstance->Toggle(true,bIsEquippable,bIsSplittable,bIsConsumable, SlotIndex);
	
	
	bIsContextMenuOpen = true;
}
void UInventoryWidget::OnContextMenuDisabled()
{
	CurrentSplittingSlotIndex = INDEX_NONE;
	bIsContextMenuOpen = false;
}

void UInventoryWidget::Consume(int32 SlotIndex)
{
	OnRequestScreenWidget_ContextWidget.Broadcast(false);
	OnRequestConsumeDelegate.Broadcast(SlotIndex);
}


// GENERAL =====================================================
//==============================================================
void UInventoryWidget::GenerateSlots()
{
	if (!InventoryComponent) return;
	if (!SlotWidget) return;

	
	if (SlotWidgets.Num() > 0)
	{
		return;
	}

	int32 SlotAmount = InventoryComponent->Capacity;
	
	for (int32 i = 0; i < SlotAmount; i++)
	{
		UInventorySlotWidget* NewSlot =
			CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), SlotWidget);

		if (NewSlot)
		{
			NewSlot->SlotIndex = i;
			NewSlot->InventoryWidget = this ;
			SlotWidgets.Add(NewSlot);
			InventoryGrid->AddChildToUniformGrid(NewSlot, i / 3, i % 3);
			
			// SLOT'S DELEGATES
			NewSlot->OnToggleSplitDelegate.AddDynamic(this, &UInventoryWidget::StartSplitting);
			
			//MERGING
			NewSlot->OnMergeStacksDelegate.AddDynamic(this, &UInventoryWidget::RequestMergeStacks);
			NewSlot->OnMergeStacksFromSplitDelegate.AddDynamic(this, &UInventoryWidget::RequestMergeStacksFromSplit);
			
			NewSlot->OnSwapDelegate.AddDynamic(this, &UInventoryWidget::RequestSwap);
			NewSlot->OnSwapFromSplitDelegate.AddDynamic(this, &UInventoryWidget::RequestSwapFromSplit);
			
			NewSlot->OnMoveOntoDelegate.AddDynamic(this, &UInventoryWidget::RequestChangeIndex);
			NewSlot->OnRightClickDelegate.AddDynamic(this, &UInventoryWidget::HandleRightClickOnSlot);
			//Forward directly to Component 
			NewSlot->OnMoveOntoFromEquipmentDelegate.AddDynamic(InventoryComponent, &UInventoryComponent::AddFromEquipment);
			NewSlot->OnSwapEquipmentItemsDelegate.AddDynamic(InventoryComponent,  &UInventoryComponent::SwapWithEquipment);
		
		}
	}
}
void UInventoryWidget::UpdateSlots()
{
	if (!InventoryComponent) return;
	
	UE_LOG(LogTemp,Warning,TEXT("UpdateSlots"));
	UpdateWeight();
	
	const TArray<FItemStack>& Items = InventoryComponent->GetItems();
	
	for (int32 i = 0; i<SlotWidgets.Num();i++)
	{
		const FItemStack& ItemStack = Items[i];
		
		if (ItemStack.ItemID != NAME_None)
		{
			const FItemData* ItemData = InventoryComponent->GetItemData(Items[i].ItemID);
			SlotWidgets[i]->SetData(Items[i], *ItemData);
		}
		else
		{
			SlotWidgets[i]->Cleanup();
		}
	}
}
