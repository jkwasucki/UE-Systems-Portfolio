// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/GenericTypes.h"
#include "InputMappingContext.h"
#include "Components/ActorComponent.h"
#include "InputHandlerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpaceDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArrowPress, EMoveDirection, Dir);

class AMainPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UInputHandlerComponent : public UActorComponent
{
	GENERATED_BODY()
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UInputHandlerComponent();
	
	UPROPERTY()
	AMainPlayerController* PC = nullptr;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* PacmanMappingContext;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputAction* INTERACT_InputAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputAction* INVENTORY_InputAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputAction* RIGHTCLICK_InputAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArrowUp;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArrowDown;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArrowLeft;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArrowRight;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Space;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Tab;
	
	
	// Delegates (Events)
	FOnInteract OnInteractDelegate;	
	FOnInventory OnInventoryDelegate;
	FOnRightClick OnRightClickDelegate;
	FOnArrowPress OnArrowPressDelegate;
	FOnSpaceDown OnSpaceDownDelegate;
	FOnTabDown OnTabDownDelegate; 
	
	UFUNCTION()
	void SetupInput(UEnhancedInputComponent* InputComponent);
	UFUNCTION()
	void SetupInputActions(UEnhancedInputComponent* EnhancedInputComponent);
	UFUNCTION()
	void SetupMappingContext();
	UFUNCTION()
	void OnInteract();
	
	UFUNCTION()
	void OnInventory();

	UFUNCTION()
	void OnRightClick();
	
	UFUNCTION()
	void  OnArrowUp();
	UFUNCTION()
	void  OnArrowDown();
	UFUNCTION()
	void  OnArrowLeft();
	UFUNCTION()
	void  OnArrowRight();
	UFUNCTION()
	void OnTabDown();
	UFUNCTION()
	void OnSpaceDown();
	UFUNCTION()
	void ActivatePacmanContext();
	UFUNCTION()
	void ActivateDefaultContext();
	
	
};
