// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Types/GenericTypes.h"
#include "InputMappingContext.h"
#include  "Types/AbilityTypes.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "InputHandlerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpaceDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInput, FGameplayTag, Tag, EAbilityInputEvent, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowPress, EMoveDirection, Dir);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityUnderCursor, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoEntityUnderCursor);
class AMainPlayerController;
UCLASS(
	ClassGroup=(Custom),
	Blueprintable,
	meta=(BlueprintSpawnableComponent)
)
class CLONE1_API UInputHandlerComponent : public UActorComponent
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	ABaseCharacter* EntityUnderCursor = nullptr;
public:	
	
	// Sets default values for this component's properties
	UInputHandlerComponent();
	
	UPROPERTY()
	AMainPlayerController* PC = nullptr;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Contexts")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Contexts")
	UInputMappingContext* PacmanMappingContext;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* INTERACT_InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* INVENTORY_InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* RIGHTCLICK_InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_ArrowUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_ArrowDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_ArrowLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_ArrowRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_Space;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_Tab;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_AbilitySlot1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_AbilitySlot2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_AbilitySlot3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_AbilitySlot4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_AbilitySlot5;
	
	// Delegates
	FOnInteract OnInteractDelegate;	
	FOnInventory OnInventoryDelegate;
	FOnRightClick OnRightClickDelegate;
	FOnArrowPress OnArrowPressDelegate;
	FOnSpaceDown OnSpaceDownDelegate;
	FOnTabDown OnTabDownDelegate; 
	FOnAbilityInput OnAbilityInputDelegate;
	FOnEntityUnderCursor OnEntityUnderCursorDelegate;
	FOnNoEntityUnderCursor OnNoEntityUnderCursorDelegate;
	
	
	UFUNCTION()
	void SetupInput(UEnhancedInputComponent* InputComponent);
	UFUNCTION()
	void SetupInputActions(UEnhancedInputComponent* inEnhancedInputComponent);
	UFUNCTION()
	void SetupMappingContext();
	UFUNCTION()
	void OnInteract();
	UFUNCTION()
	void ProcessResultUnderCursor(bool bHit,  const FHitResult& Hit);
	
	
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
	
	UFUNCTION()
	void OnInventory();
	UFUNCTION()
	void OnRightClick();
	UFUNCTION()
	void OnTabDown();
	UFUNCTION()
	void OnSpaceDown();
	UFUNCTION()
	void ActivatePacmanContext();
	UFUNCTION()
	void ActivateDefaultContext();

	
	
};
