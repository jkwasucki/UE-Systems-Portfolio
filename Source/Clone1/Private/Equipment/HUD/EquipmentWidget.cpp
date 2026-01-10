// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentWidget.h"
#include "Equipment/HUD/EquipmentSlotWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "Blueprint/WidgetTree.h"

void UEquipmentWidget::Init(UInventoryScreenWidget* inOrchestrator)
{
	if (bIsInitialized) return;
	
	bIsInitialized = true;
	
	if (inOrchestrator)
	{
		Orchestrator = inOrchestrator;
		if (inOrchestrator->PC)
		{
			EquipmentComponent = inOrchestrator->PC->GetPlayerState<AMainPlayerState>()->EquipmentComponent;
			
			EquipmentComponent->OnEquipmentChanged.AddDynamic(this,&UEquipmentWidget::RefreshEquipment);
			
			//Bind delegates to EquipmentComponent API methods
			//
			//
			//
			for (const TPair<EEquipmentType, UEquipmentSlotWidget*>& ES : EquipmentSlots)
			{
				UEquipmentSlotWidget* W = ES.Value;
				
				W->OnSwapWithInventoryDelegate.AddDynamic(EquipmentComponent, &UEquipmentComponent::SwapWithInventory);
				W->OnDroppedOnto_FromInventory.AddDynamic(EquipmentComponent,&UEquipmentComponent::AddFromInventory);
				W->OnRightClickDelegate.AddDynamic(EquipmentComponent, &UEquipmentComponent::MoveToInventory);
			}
		}
		
		RefreshEquipment();
	}
}



void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TArray<UWidget*> FoundSlots;
	WidgetTree->ForEachWidget([this] (UWidget* widget)
		{
			if (UEquipmentSlotWidget* S = Cast<UEquipmentSlotWidget>(widget))
			{
				EquipmentSlots.Add(S->EquipmentType,S);
				S->EquipmentWidget = this;
			}
		}		
	
	);
	
	
}

void UEquipmentWidget::Enable()
{
	RefreshEquipment();
}

void UEquipmentWidget::Disable()
{
}

void UEquipmentWidget::RefreshEquipment()
{
	for (const TPair<EEquipmentType, UEquipmentSlotWidget*>& ES : EquipmentSlots)
	{
		if (const FItemData* Data = EquipmentComponent->GetEquipmentDataBySlot(ES.Key))
		{
			ES.Value->SetData(*Data);
		}
		else
		{
			ES.Value->Cleanup();
		}
	}
}
