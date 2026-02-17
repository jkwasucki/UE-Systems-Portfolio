// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/CharacterInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UCharacterInputComponent::UCharacterInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterInputComponent::Init(AMainPlayerController* InPC,UEnhancedInputComponent* EI )
{
	if (!IsValid(InPC)) return;
	PC = InPC;
	EnhancedInputComponent = EI;
	SetupMappingContext();
	SetupInput(EI);
}
void UCharacterInputComponent::SetupInput(UEnhancedInputComponent* InputComponent)
{
	if (PlayerMappingContext)
	{
		// To bind Input Actions, use EhnancedInputComponent that comes from InputComponent
		SetupInputActions(InputComponent);
	}
}

void UCharacterInputComponent::SetupInputActions(UEnhancedInputComponent* InEnhancedInputComponent)
{
	if (InEnhancedInputComponent)
	{
		EnhancedInputComponent = InEnhancedInputComponent;
		
		if (IA_Interact)
			EnhancedInputComponent->BindAction(IA_Interact,ETriggerEvent::Started,this, &UCharacterInputComponent::Input_Interact);
		
		if (IA_ToggleInventory)
			EnhancedInputComponent->BindAction(IA_ToggleInventory,ETriggerEvent::Started,this,&UCharacterInputComponent::Input_Inventory);
		
		
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

void UCharacterInputComponent::SetupMappingContext()
{
	if (PlayerMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			//To setup mapping context, use LocalInput subsystem
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedLocalInput = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedLocalInput->AddMappingContext(PlayerMappingContext,0);
			}
		}
	}
}



void UCharacterInputComponent::BindAbilityInput(UEnhancedInputComponent* Input, UInputAction* Action,
	const FGameplayTag& SlotTag)
{
	Input->BindAction(
		Action,
		ETriggerEvent::Started,
		this,
		&UCharacterInputComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Started
	);

	Input->BindAction(
		Action,
		ETriggerEvent::Canceled,
		this,
		&UCharacterInputComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Canceled
	);

	Input->BindAction(
		Action,
		ETriggerEvent::Completed,
		this,
		&UCharacterInputComponent::HandleAbilityInput,
		SlotTag,
		EAbilityInputEvent::Completed
	);
}

void UCharacterInputComponent::BindArrowInput(UEnhancedInputComponent* Input, UInputAction* Action,
	EMoveDirection Direction)
{
	Input->BindAction(
		Action,
		ETriggerEvent::Started,
		this,
		&UCharacterInputComponent::HandleArrowInput,
		Direction
		);
}



void UCharacterInputComponent::HandleAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event) {OnAbilityInputDelegate.Broadcast(SlotTag, Event);}

void UCharacterInputComponent::HandleArrowInput(EMoveDirection Direction) {OnArrowPressDelegate.Broadcast(Direction);}

void UCharacterInputComponent::Input_Inventory() {OnInventoryDelegate.Broadcast();}

void UCharacterInputComponent::Input_Interact() {OnInteractDelegate.Broadcast();}


