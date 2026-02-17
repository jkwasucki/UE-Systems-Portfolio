// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/InputHandlerComponent.h"
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
	if (PC.IsValid()) return;

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
	if (PC.IsValid()) return;
		
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
void UInputHandlerComponent::SetupInputActions(UEnhancedInputComponent* inEnhancedInputComponent)
{
	if (inEnhancedInputComponent)
	{
		EnhancedInputComponent = inEnhancedInputComponent;
		
	
		if (IA_RightClick)
			EnhancedInputComponent->BindAction(IA_RightClick,ETriggerEvent::Started,this, &UInputHandlerComponent::OnRightClick);
		
		if (IA_Space)
			EnhancedInputComponent->BindAction(IA_Space,ETriggerEvent::Started,this,&UInputHandlerComponent::OnSpaceDown);
		
		if (IA_Tab)
			EnhancedInputComponent->BindAction(IA_Tab,ETriggerEvent::Started,this,&UInputHandlerComponent::OnTabDown);
		
		if (IA_SaveGame)
			EnhancedInputComponent->BindAction(IA_SaveGame,ETriggerEvent::Started,this,&UInputHandlerComponent::OnSaveGame);
		
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


void UInputHandlerComponent::OnRightClick()
{
	OnRightClickDelegate.Broadcast();
}


void UInputHandlerComponent::OnTabDown()
{
	OnTabDownDelegate.Broadcast();
}

void UInputHandlerComponent::OnSpaceDown()
{
	OnSpaceDownDelegate.Broadcast();
}

void UInputHandlerComponent::OnSaveGame()
{
	OnInputSaveGameDelegate.Broadcast();
}


