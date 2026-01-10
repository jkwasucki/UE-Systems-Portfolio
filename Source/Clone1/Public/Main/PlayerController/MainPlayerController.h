// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputHandlerComponent.h"
#include "GameFramework/PlayerController.h"
#include "InteractionComponent.h"
#include "HUDComponent.h"
#include "MainPlayerController.generated.h"
class UEnhancedInputComponent;
class AArcadeMachine;
UCLASS()
class CLONE1_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	
public:
	AMainPlayerController();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInputHandlerComponent* InputHandlerComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractionComponent* InteractionComponent = nullptr;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UHUDComponent* HUDComponent = nullptr;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemsDataTable = nullptr;
	
	UFUNCTION()
	void SpawnItemActor(FItemStack ItemStack);
	
	UFUNCTION()
	FVector GetDropLocation();
	UFUNCTION()
	void StartPacmanGame(AArcadeMachine* PG);
	UFUNCTION()
	void StopArcade();
};


