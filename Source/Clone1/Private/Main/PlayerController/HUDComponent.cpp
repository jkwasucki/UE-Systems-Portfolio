// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/HUDComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/InteractableInterface.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Main/Character/Derived/MainCharacter.h"

UHUDComponent::UHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UHUDComponent::Init(AMainPlayerController* InPC)
{
	if (!InPC) return;
	PC = InPC;
	
	SetupHUD();
	PassReferences();
	
	AMainCharacter* MainCharacter = PC->GetMainCharacter();
	if (MainCharacter)
	{
		MainCharacter->GetCharacterInput()->OnInventoryDelegate.AddDynamic(this, &UHUDComponent::OnToggleInventory);
		MainCharacter->GetInteractionComponent()->OnInteractableFocused.AddDynamic(this,&UHUDComponent::OnNewInteractable);
		MainCharacter->GetInteractionComponent()->OnInteractableFocusEndDelegate.AddDynamic(this,&UHUDComponent::HideInteractionTooltip);
	}
}


void UHUDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInteractableTooltipLocation();
}

void UHUDComponent::HandleInteractionTooltip()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("NOT NULL"));
	if (CurrentInteractable.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("NOT NULL1"));
		if (!IInteractableInterface::Execute_IsInteractedWith(CurrentInteractable.Get()))
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("NOT NULL2"));
			ShowInteractionTooltip(CurrentInteractable.Get());
			return;
		}
		HideInteractionTooltip();
		return;
	}
	
	if (CurrentInteractable == nullptr)
		HideInteractionTooltip();
}

void UHUDComponent::UpdateInteractableTooltipLocation()
{
	if (CurrentInteractable.IsValid())
	{
		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(PC.Get(),CurrentInteractable.Get()->GetActorLocation(),ScreenPos);
		
		InteractionTooltipWidget->SetPositionInViewport(ScreenPos);
	}
}

void UHUDComponent::OnNewInteractable(UInteractionDefinition* Definition, AActor* Interactable)
{
	
	CurrentInteractable = Interactable;
	InteractionDefinition = Definition;
	
	HandleInteractionTooltip();
}

void UHUDComponent::SetupHUD()
{
	if (InventoryScreenHUDComponent)
	{
		InventoryScreenWidget = CreateWidget<UInventoryScreenWidget>(PC.Get(), InventoryScreenHUDComponent);
		if (InventoryScreenWidget)
		{
			InventoryScreenWidget->AddToViewport();
			InventoryScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (InteractionTooltipHUDComponent)
	{
		InteractionTooltipWidget = CreateWidget<UInteractionTooltipWidget>(PC.Get(), InteractionTooltipHUDComponent);
		if (InteractionTooltipWidget)
		{
			InteractionTooltipWidget->AddToViewport();
			InteractionTooltipWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (ScreenGameplayDebugHUDComponent)
	{
		ScreenGameplayDebugWidget = CreateWidget<UScreenGameplayDebugWidget>(PC.Get(), ScreenGameplayDebugHUDComponent);
		if (ScreenGameplayDebugWidget)
		{
			ScreenGameplayDebugWidget->AddToViewport();
			ScreenGameplayDebugWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}



void UHUDComponent::OnToggleInventory()
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

void UHUDComponent::ShowInteractionTooltip(AActor* inFocusedActor)
{
	if (!InteractionDefinition.IsValid()) return;
	InteractionTooltipWidget->SetVisibility(ESlateVisibility::Visible);
	InteractionTooltipWidget->SetDataBP(FText::FromString("E"), InteractionDefinition->Prompt);
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
		InventoryScreenWidget->Init(PC.Get());
	}
	if (ScreenGameplayDebugWidget)
	{
		ScreenGameplayDebugWidget->Init(PC.Get());
	}
}

