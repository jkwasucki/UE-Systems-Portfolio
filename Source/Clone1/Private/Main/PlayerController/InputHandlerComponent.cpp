// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/InputHandlerComponent.h"
#include "Types/GenericTypes.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Main/PlayerController/MainPlayerController.h"

void UInputHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<AMainPlayerController>(GetOwner());
	SetupMappingContext();
}

UInputHandlerComponent::UInputHandlerComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	

}


// MAPPING CONTEXT SWITCHES
void UInputHandlerComponent::ActivatePacmanContext()
{
	if (!PC) return;

	if (DefaultMappingContext && PacmanMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{	
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedLocalInput = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedLocalInput->RemoveMappingContext(DefaultMappingContext);
				EnhancedLocalInput->AddMappingContext(PacmanMappingContext,10);
			}
		}
	}
	
}
void UInputHandlerComponent::ActivateDefaultContext()
{
	if (!PC) return;
		
	if (DefaultMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedLocalInput = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedLocalInput->RemoveMappingContext(PacmanMappingContext);
				EnhancedLocalInput->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
}



// SETUP
void UInputHandlerComponent::SetupInput(UEnhancedInputComponent* InputComponent)
{
	if (DefaultMappingContext)
	{
		// To bind Input Actions, use EhnancedInputComponent that comes from InputComponent
		SetupInputActions(InputComponent);
	}
}
void UInputHandlerComponent::SetupInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (EnhancedInputComponent)
	{
		if (INTERACT_InputAction)
		{
			EnhancedInputComponent->BindAction(INTERACT_InputAction,ETriggerEvent::Started,this, &UInputHandlerComponent::OnInteract);
		}
		if (INVENTORY_InputAction)
		{
			EnhancedInputComponent->BindAction(INVENTORY_InputAction,ETriggerEvent::Started,this,&UInputHandlerComponent::OnInventory);
		}
		if (RIGHTCLICK_InputAction)
		{
			EnhancedInputComponent->BindAction(RIGHTCLICK_InputAction,ETriggerEvent::Started,this, &UInputHandlerComponent::OnRightClick);
		}
		if (IA_ArrowDown)
		{
			EnhancedInputComponent->BindAction(IA_ArrowDown,ETriggerEvent::Started,this,&UInputHandlerComponent::OnArrowDown);
		}
		if (IA_ArrowUp)
		{
			EnhancedInputComponent->BindAction(IA_ArrowUp,ETriggerEvent::Started,this,&UInputHandlerComponent::OnArrowUp);
		}
		if (IA_ArrowLeft)
		{
			
			EnhancedInputComponent->BindAction(IA_ArrowLeft,ETriggerEvent::Started,this,&UInputHandlerComponent::OnArrowLeft);
		}
		if (IA_ArrowRight)
		{
			EnhancedInputComponent->BindAction(IA_ArrowRight,ETriggerEvent::Started,this,&UInputHandlerComponent::OnArrowRight);
		}
		if (IA_Space)
		{
			EnhancedInputComponent->BindAction(IA_Space,ETriggerEvent::Started,this,&UInputHandlerComponent::OnSpaceDown);
		}
		if (IA_Tab)
		{
			EnhancedInputComponent->BindAction(IA_Tab,ETriggerEvent::Started,this,&UInputHandlerComponent::OnTabDown);
		}
	}
}

void UInputHandlerComponent::SetupMappingContext()
{
	if (DefaultMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			//To setup mapping context, use LocalInput subsystem
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedLocalInput = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedLocalInput->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
}


void UInputHandlerComponent::OnInteract()
{
	OnInteractDelegate.Broadcast();
}

void UInputHandlerComponent::OnInventory()
{
	
	OnInventoryDelegate.Broadcast();
}

void UInputHandlerComponent::OnRightClick()
{
	OnRightClickDelegate.Broadcast();
}
void UInputHandlerComponent::OnArrowUp()
{

	OnArrowPressDelegate.Broadcast(EMoveDirection::UP);
}

void UInputHandlerComponent::OnArrowDown()
{

	OnArrowPressDelegate.Broadcast(EMoveDirection::DOWN);
}

void UInputHandlerComponent::OnArrowLeft()
{
	
	OnArrowPressDelegate.Broadcast(EMoveDirection::LEFT);
}

void UInputHandlerComponent::OnArrowRight()
{
	
	OnArrowPressDelegate.Broadcast(EMoveDirection::RIGHT);
}

void UInputHandlerComponent::OnTabDown()
{
	OnTabDownDelegate.Broadcast();
}

void UInputHandlerComponent::OnSpaceDown()
{
	OnSpaceDownDelegate.Broadcast();
}


