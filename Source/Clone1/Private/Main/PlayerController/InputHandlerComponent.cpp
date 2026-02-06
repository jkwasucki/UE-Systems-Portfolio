// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/InputHandlerComponent.h"

#include "Main/Character/Base/BaseCharacter.h"
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
void UInputHandlerComponent::SetupInputActions(UEnhancedInputComponent* inEnhancedInputComponent)
{
	if (inEnhancedInputComponent)
	{
		EnhancedInputComponent = inEnhancedInputComponent;
		
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
		if (IA_Space)
		{
			EnhancedInputComponent->BindAction(IA_Space,ETriggerEvent::Started,this,&UInputHandlerComponent::OnSpaceDown);
		}
		if (IA_Tab)
		{
			EnhancedInputComponent->BindAction(IA_Tab,ETriggerEvent::Started,this,&UInputHandlerComponent::OnTabDown);
		}
		
		BindArrowInput(
			EnhancedInputComponent,
			IA_ArrowLeft,
			EMoveDirection::LEFT
		);
		BindArrowInput(
			EnhancedInputComponent,
			IA_ArrowRight,
			EMoveDirection::RIGHT
		);
		BindArrowInput(
			EnhancedInputComponent,
			IA_ArrowUp,
			EMoveDirection::UP
		);
		BindArrowInput(
			EnhancedInputComponent,
			IA_ArrowDown,
			EMoveDirection::DOWN
		);
		
		BindAbilityInput(
			EnhancedInputComponent,
			IA_AbilitySlot1,
			FGameplayTag::RequestGameplayTag(TEXT("Abilities.Slot.Key1"))
		);

		BindAbilityInput(
			EnhancedInputComponent,
			IA_AbilitySlot2,
			FGameplayTag::RequestGameplayTag(TEXT("Abilities.Slot.Key2"))
		);

		BindAbilityInput(
			EnhancedInputComponent,
			IA_AbilitySlot3,
			FGameplayTag::RequestGameplayTag(TEXT("Abilities.Slot.Key3"))
		);
		BindAbilityInput(
			EnhancedInputComponent,
			IA_AbilitySlot4,
			FGameplayTag::RequestGameplayTag(TEXT("Abilities.Slot.Key4"))
		);
		BindAbilityInput(
			EnhancedInputComponent,
			IA_AbilitySlot5,
			FGameplayTag::RequestGameplayTag(TEXT("Abilities.Slot.Key5"))
		);
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



void UInputHandlerComponent::BindAbilityInput(UEnhancedInputComponent* Input, UInputAction* Action,
	const FGameplayTag& SlotTag)
{
	Input->BindAction(
		Action,
		ETriggerEvent::Started,
		this,
		&UInputHandlerComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Started
	);

	Input->BindAction(
		Action,
		ETriggerEvent::Canceled,
		this,
		&UInputHandlerComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Canceled
	);

	Input->BindAction(
		Action,
		ETriggerEvent::Completed,
		this,
		&UInputHandlerComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Completed
	);
}
void UInputHandlerComponent::BindArrowInput(UEnhancedInputComponent* Input, UInputAction* Action,
	EMoveDirection Direction)
{
	Input->BindAction(
		Action,
		ETriggerEvent::Started,
		this,
		&UInputHandlerComponent::HandleArrowInput,
		Direction
		);
}



void UInputHandlerComponent::OnInteract()
{
	OnInteractDelegate.Broadcast();
}

void UInputHandlerComponent::ProcessResultUnderCursor(bool bHit,  const FHitResult& Hit)
{
	bool bHitPawn = false;
	APawn* Pawn = nullptr;
	if (bHit)
	{
		Pawn = Cast<APawn>(Hit.GetActor());
		if (Pawn)
			bHitPawn = true;
	}
	
	if (!bHitPawn)
	{
		if (EntityUnderCursor)
		{
			EntityUnderCursor = nullptr;
			OnNoEntityUnderCursorDelegate.Broadcast();
		}
		return;
	}
	
	if (ABaseCharacter* Char = Cast<ABaseCharacter>(Pawn))
	{
		if (Char != EntityUnderCursor)
			OnEntityUnderCursorDelegate.Broadcast(Hit.GetActor());
		
		EntityUnderCursor = Char;
		return;
	}

	if (EntityUnderCursor != nullptr)
	{
		EntityUnderCursor = nullptr;
		OnNoEntityUnderCursorDelegate.Broadcast();
	}
}

void UInputHandlerComponent::HandleAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event)
{
	OnAbilityInputDelegate.Broadcast(SlotTag, Event);
}

void UInputHandlerComponent::HandleArrowInput(EMoveDirection Direction)
{
	OnArrowPressDelegate.Broadcast(Direction);
}

void UInputHandlerComponent::OnInventory()
{
	
	OnInventoryDelegate.Broadcast();
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


