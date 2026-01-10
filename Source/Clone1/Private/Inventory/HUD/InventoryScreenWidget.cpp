// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HUD/InventoryScreenWidget.h"

#include "Equipment/HUD/EquipmentAttributesWidget.h"
#include "Equipment/HUD/EquipmentWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Inventory/HUD/InventoryWidget.h"
#include "Components/CanvasPanelSlot.h"

void UInventoryScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UInventoryScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (PendingContextWidget)
	{
		PositionWidgetAtCursor(PendingContextWidget);
		PendingContextWidget = nullptr;
	}
	
	if (!bIsDragWidgetActive) return;
	
	float MouseX,MouseY;
	if (PC->GetMousePosition(MouseX,MouseY))
	{
		FVector2D MousePos = FVector2D(MouseX,MouseY);
		DraggedWidgetInstance->SetPositionInViewport(MousePos,true);
	}
}


void UInventoryScreenWidget::DisableModals()
{
	if (bIsContexWidgetActive)
	{
		ToggleContextWidget(false);
	}
	if (bIsSplitWidgetActive)
	{
		ToggleSplitWidget(false);
	}
	
	if (bIsDragWidgetActive)
	{
		ToggleDragWidget(false);
	}
	PrevMousePos = FVector2D(0.f,0.f);
	PopupLayer->SetVisibility(ESlateVisibility::Collapsed);
	OnModalsDisabledDelegate.Broadcast();
}



void UInventoryScreenWidget::Toggle()
{
	if (!InventoryWidget) return;
	
	bIsActive = !bIsActive;
	
	if (bIsActive)
	{
		
		AttributesWidget->Init(this);
		AttributesWidget->Toggle();
		
		
		InventoryWidget->Init(this);
		InventoryWidget->Enable();
		
		EquipmentWidget->Init(this);
		EquipmentWidget->Enable();
		
	}
	else
	{
		DisableModals();
		
		InventoryWidget->Disable();
		EquipmentWidget->Disable();
	}
}

void UInventoryScreenWidget::Init(AMainPlayerController* PlayerController)
{
	if (PlayerController)
	{
		PC = PlayerController;
		if (PC->InputHandlerComponent)
		{
			PC->InputHandlerComponent->OnRightClickDelegate.AddDynamic(this, &UInventoryScreenWidget::PassRightClickEvent);
		}
	
		
		if (InventoryContextMenuComponent)
		{
			InventoryContextMenuInstance = CreateWidget<UInventoryContextMenuWidget>(PC, InventoryContextMenuComponent);
			if (InventoryContextMenuInstance)
			{
				InventoryContextMenuInstance->ForceLayoutPrepass();
				InventoryContextMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		
		if (InventoryItemSplitWidgetComponent)
		{
			InventorySplitWidget = CreateWidget<UInventorySlotSplitWidget>(PC, InventoryItemSplitWidgetComponent);
			if (InventorySplitWidget)
			{
				InventoryContextMenuInstance->ForceLayoutPrepass();
				InventorySplitWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	
		if (DraggedWidgetComponent)
		{
			DraggedWidgetInstance = CreateWidget<UInventorySlotDraggedWidget>(PC, DraggedWidgetComponent);

			if (DraggedWidgetInstance)
			{
				DraggedWidgetInstance->AddToViewport();
				DraggedWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
				DraggedWidgetInstance->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
			}
		}
		
		// Add modals to popup layer to detect outside-clicks
		if (PopupLayer)
		{
			PopupCanvas->AddChild(InventoryContextMenuInstance);
			PopupCanvas->AddChild(InventorySplitWidget);
			
			PopupLayer->OnClicked.AddDynamic(this, &UInventoryScreenWidget::OnPopupLayerClick);
		}
	
		// Pass Widget references
		InventoryWidget->SetupContextMenu(InventoryContextMenuInstance);
		InventoryWidget->SetupSplitWidget(InventorySplitWidget);
	
		InventoryWidget->OnRequestScreenWidget_SplitWidget.AddDynamic(this, &UInventoryScreenWidget::ToggleSplitWidget);
		InventoryWidget->OnRequestScreenWidget_ContextWidget.AddDynamic(this, &UInventoryScreenWidget::ToggleContextWidget);
		InventoryWidget->OnRequestScreenWidget_DragWidget.AddDynamic(this, &UInventoryScreenWidget::ToggleDragWidget);
	}
	
}
void UInventoryScreenWidget::OnPopupLayerClick()
{
	DisableModals();
}

void UInventoryScreenWidget::PositionWidgetAtCursor(UWidget* Widget)
{
	
	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);

	const float ViewportScale =
		UWidgetLayoutLibrary::GetViewportScale(this);

	const FVector2D MousePos(
		MouseX / ViewportScale,
		MouseY / ViewportScale
	);

	const FVector2D Offset(8.f, 8.f); // optional

	if (UCanvasPanelSlot* s = Cast<UCanvasPanelSlot>(Widget->Slot))
	{
		s->SetAnchors(FAnchors(0.f, 0.f));
		s->SetAlignment(FVector2D(0.f, 0.f));
		s->SetPosition(PrevMousePos != FVector2D(0.f,0.f) ? PrevMousePos + Offset : MousePos + Offset);
		
		PrevMousePos = MousePos;
	}
}



void UInventoryScreenWidget::PassRightClickEvent()
{
	OnRightClickDelegate.Broadcast();
}



void UInventoryScreenWidget::ToggleSplitWidget(bool bState)
{
	bIsSplitWidgetActive =  bState ? true : false;
	if (bState)
	{
		PopupLayer->SetVisibility(ESlateVisibility::Visible);
		InventorySplitWidget->SetVisibility(ESlateVisibility::Visible);
		PositionWidgetAtCursor(InventorySplitWidget);
	}
	else
	{
		PrevMousePos = FVector2D(0.f,0.f);
		PopupLayer->SetVisibility(ESlateVisibility::Collapsed);
		InventorySplitWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
void UInventoryScreenWidget::ToggleContextWidget(bool bState)
{
	bIsContexWidgetActive =  bState ? true : false;
	if (bState)
	{
		PopupLayer->SetVisibility(ESlateVisibility::Visible);
		InventoryContextMenuInstance->SetVisibility(ESlateVisibility::Visible);
		PositionWidgetAtCursor(InventoryContextMenuInstance);
	}
	else
	{
		PrevMousePos = FVector2D(0.f,0.f);
		PopupLayer->SetVisibility(ESlateVisibility::Collapsed);
		InventoryContextMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void UInventoryScreenWidget::ToggleDragWidget(bool bState)
{
	
	bIsDragWidgetActive = bState ? true : false;
	DraggedWidgetInstance->SetVisibility(bState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}


