// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/PlayerController/InputHandlerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Main/PlayerController/HUDComponent.h"
#include "MainPlayerController.generated.h"


class AMainCharacter;
class UEnhancedInputComponent;
class AArcadeMachine;

UCLASS()
class CLONE1_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	
	
	/// TODO: FIX ITEM DATA TABLES BEING IN SO MANY CLASSES (MOVE TO INVENTORY ONLY - QUERY FROM THERE)
	
	// COMPONENTS
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInputHandlerComponent* InputHandlerComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UHUDComponent* HUDComponent;
	
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ItemsDataTable = nullptr;
	
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	AMainPlayerController();
	UFUNCTION()
	void SpawnItemActor(FItemStack ItemStack);
	UFUNCTION()
	void StartPacmanGame(AArcadeMachine* PG);
	UFUNCTION()
	void StopArcade();
	
	
	// GETTERS / QUERIES
	UFUNCTION()
	FVector GetDropLocation();
	UFUNCTION()
	FVector GetCharacterLocation();
	UFUNCTION(BlueprintPure)
	UHUDComponent* GetHUDComponent() const;
	UFUNCTION()
	AMainCharacter* GetMainCharacter() const;
	UFUNCTION()
	void RequestSaveGame();
};


