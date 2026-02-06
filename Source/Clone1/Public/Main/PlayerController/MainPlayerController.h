// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/PlayerController/InputHandlerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Main/PlayerController/InteractionComponent.h"
#include "Main/PlayerController/HUDComponent.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntityDebugSnapshot,ABaseCharacter*, BaseCharacter, FEntityGameplayDebugSnapshot, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEntityDebugSnapshot_Expired);

class UEnhancedInputComponent;
class AArcadeMachine;
UCLASS()
class CLONE1_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	AMainPlayerController();
	
	
	// COMPONENTS
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInputHandlerComponent* InputHandlerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UHUDComponent* HUDComponent;
	
	
	// DELEGATES
	FOnEntityDebugSnapshot OnEntityDebugSnapshotDelegate;
	FOnEntityDebugSnapshot_Expired OnEntityDebugSnapshot_ExpiredDelegate;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ItemsDataTable = nullptr;
	
	UFUNCTION()
	void SpawnItemActor(FItemStack ItemStack);
	

	UFUNCTION()
	void SetEntityDataExpired();
	
	
	UFUNCTION()
	void StartPacmanGame(AArcadeMachine* PG);
	UFUNCTION()
	void StopArcade();
	
	
	// GETTERS / QUERIES
	UFUNCTION()
	void GetEntityData(AActor* Actor);
	UFUNCTION()
	FVector GetDropLocation();
	UFUNCTION()
	void QueryEntityUnderCursor();
	UFUNCTION()
	FVector GetCharacterLocation();
	UFUNCTION(BlueprintPure)
	UHUDComponent* GetHUDComponent() const;
};


