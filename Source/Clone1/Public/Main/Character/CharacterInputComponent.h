// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Base/BaseCharacter.h"
#include "Components/ActorComponent.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "CharacterInputComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInput, FGameplayTag, Tag, EAbilityInputEvent, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowPress, EMoveDirection, Dir);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UCharacterInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Contexts")
	UInputMappingContext* PlayerMappingContext;
	
	
	FOnInteract OnInteractDelegate;	
	FOnInventory OnInventoryDelegate;
	FOnAbilityInput OnAbilityInputDelegate;
	FOnArrowPress OnArrowPressDelegate;

	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Interaction")
	UInputAction* IA_Interact;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Inventory")
	UInputAction* IA_ToggleInventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Ability")
	UInputAction* IA_AbilitySlot1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Ability")
	UInputAction* IA_AbilitySlot2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Ability")
	UInputAction* IA_AbilitySlot3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Ability")
	UInputAction* IA_AbilitySlot4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Ability")
	UInputAction* IA_AbilitySlot5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Arrows")
	UInputAction* IA_ArrowUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Arrows")
	UInputAction* IA_ArrowDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Arrows")
	UInputAction* IA_ArrowLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Arrows")
	UInputAction* IA_ArrowRight;

protected:
	UPROPERTY()
	TWeakObjectPtr<AMainPlayerController> PC;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;
	
public:
	UCharacterInputComponent();

	UFUNCTION()
	void Init(AMainPlayerController* InPC,UEnhancedInputComponent* EI );
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void SetupInput(UEnhancedInputComponent* InputComponent);
	UFUNCTION()
	void SetupMappingContext();
	UFUNCTION()
	void SetupInputActions(UEnhancedInputComponent* InEnhancedInputComponent);
	UFUNCTION()
	void Input_Inventory();
	UFUNCTION()
	void Input_Interact();
	UFUNCTION()
	void BindAbilityInput(
		UEnhancedInputComponent* Input,
		UInputAction* Action,
		const FGameplayTag& SlotTag
	);
	UFUNCTION()
	void HandleAbilityInput(
		FGameplayTag SlotTag,
		EAbilityInputEvent Event
	);

	UFUNCTION()
	void BindArrowInput(
		UEnhancedInputComponent* Input,
		UInputAction* Action, 
		EMoveDirection Direction
	);
	UFUNCTION()
	void HandleArrowInput(EMoveDirection Direction);
};
