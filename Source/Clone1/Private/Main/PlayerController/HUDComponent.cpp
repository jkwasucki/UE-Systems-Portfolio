// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/HUDComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/InteractableInterface.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UHUDComponent::UHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UHUDComponent::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<AMainPlayerController>(GetOwner());
	SetupHUD();
	PassReferences();
	
	SetComponentTickEnabled(true);
	PC->InputHandlerComponent->OnInventoryDelegate.AddDynamic(this, &UHUDComponent::ToggleInventory);
}


void UHUDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UInteractableComponent* NewFocusedActor = PC->InteractionComponent->GetBestInteractable();
	FocusedActorComponent = NewFocusedActor;
	HandleInteractionTooltip();
	
	
	UpdateInteractableTooltipLocation();
}

void UHUDComponent::HandleInteractionTooltip()
{
	if (FocusedActorComponent && !FocusedActorComponent->bIsInteractedWith)
	{
		if (PC->InteractionComponent->NearbyInteractables.Num() == 0 )
		{
			HideInteractionTooltip();
			return;
		}
		ShowInteractionTooltip(FocusedActorComponent);
	}
	else
	{
		HideInteractionTooltip();
	}
}




void UHUDComponent::UpdateInteractableTooltipLocation()
{
	if (FocusedActorComponent)
	{
		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(PC,FocusedActorComponent->GetOwner()->GetActorLocation(),ScreenPos);
		
		InteractionTooltipWidget->SetPositionInViewport(ScreenPos);
		
	}
}

void UHUDComponent::SetupHUD()
{
	if (InventoryScreenHUDComponent)
	{
		InventoryScreenWidget = CreateWidget<UInventoryScreenWidget>(PC, InventoryScreenHUDComponent);
		if (InventoryScreenWidget)
		{
			InventoryScreenWidget->AddToViewport();
			InventoryScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (InteractionTooltipHUDComponent)
	{
		InteractionTooltipWidget = CreateWidget<UInteractionTooltipWidget>(PC, InteractionTooltipHUDComponent);
		if (InteractionTooltipWidget)
		{
			InteractionTooltipWidget->AddToViewport();
			InteractionTooltipWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (ScreenGameplayDebugHUDComponent)
	{
		ScreenGameplayDebugWidget = CreateWidget<UScreenGameplayDebugWidget>(PC, ScreenGameplayDebugHUDComponent);
		if (ScreenGameplayDebugWidget)
		{
			ScreenGameplayDebugWidget->AddToViewport();
			ScreenGameplayDebugWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}



void UHUDComponent::ToggleInventory()
{
	if (InventoryScreenWidget->IsVisible())
	{
		PC->SetShowMouseCursor(false);

		
		//Prevent camera rotation
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
		
		FInputModeGameOnly GAMEONLYInputMode;
		PC->SetInputMode(GAMEONLYInputMode);
		
		InventoryScreenWidget->Toggle();
		InventoryScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PC->SetShowMouseCursor(true);

		
		//Prevent camera rotation
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
		
		FInputModeGameAndUI GAMEANDUIInputMode;
		PC->SetInputMode(GAMEANDUIInputMode);
		
		InventoryScreenWidget->Toggle();
		InventoryScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUDComponent::ShowInteractionTooltip(UInteractableComponent* inFocusedActor)
{
	FText KeyText = IInteractableInterface::Execute_GetActionKeyString(inFocusedActor);
	FText ActionText = IInteractableInterface::Execute_GetActionText(inFocusedActor);

	InteractionTooltipWidget->SetVisibility(ESlateVisibility::Visible);
	InteractionTooltipWidget->SetDataBP(KeyText, ActionText);
}

void UHUDComponent::HideInteractionTooltip()
{
	if (InteractionTooltipWidget->IsVisible())
	{
		InteractionTooltipWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHUDComponent::PassReferences()
{
	if (InventoryScreenWidget)
	{
		InventoryScreenWidget->Init(PC);
	}
	if (ScreenGameplayDebugWidget)
	{
		ScreenGameplayDebugWidget->Init(PC);
	}
}

