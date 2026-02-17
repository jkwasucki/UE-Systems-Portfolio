// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Components/ActorComponent.h"
#include "InputHandlerComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpaceDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTabDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputSaveGame);

class AMainPlayerController;
UCLASS(
	ClassGroup=(Custom),
	Blueprintable,
	meta=(BlueprintSpawnableComponent)
)
class CLONE1_API UInputHandlerComponent : public UActorComponent
{

	GENERATED_BODY()

protected:
	
	UFUNCTION()
	void SetupInputActions(UEnhancedInputComponent* inEnhancedInputComponent);
	UFUNCTION()
	void SetupMappingContext();
	UFUNCTION()
	void OnRightClick();
	UFUNCTION()
	void OnTabDown();
	UFUNCTION()
	void OnSpaceDown();
	UFUNCTION()
	void OnSaveGame();
	UFUNCTION()
	void ActivatePacmanContext();
	UFUNCTION()
	void ActivateDefaultContext();
	virtual void BeginPlay() override;
public:	
	
	// Sets default values for this component's properties
	UInputHandlerComponent();
	UFUNCTION()
	void SetupInput(UEnhancedInputComponent* InputComponent);
	UPROPERTY()
	TWeakObjectPtr<AMainPlayerController> PC = nullptr;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Contexts")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Contexts")
	UInputMappingContext* PacmanMappingContext;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_RightClick;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_Space;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_Tab;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* IA_SaveGame;
	
	
	// Delegates
	FOnRightClick OnRightClickDelegate;
	FOnSpaceDown OnSpaceDownDelegate;
	FOnTabDown OnTabDownDelegate; 
	FOnInputSaveGame OnInputSaveGameDelegate;
	
	
};
