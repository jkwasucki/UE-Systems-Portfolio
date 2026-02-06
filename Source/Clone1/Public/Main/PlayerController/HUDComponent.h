// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OtherHUD/GameplayDebug/ScreenGameplayDebugWidget.h"
#include "Components/InteractableComponent.h"
#include "OtherHUD/InteractionTooltipWidget.h"
#include "Inventory/HUD/InventoryScreenWidget.h"
#include "Components/ActorComponent.h"
#include "HUDComponent.generated.h"

class AMainPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	UInteractableComponent* FocusedActorComponent = nullptr;
	
	
public:	
	UHUDComponent();
	
	
	UPROPERTY()
	AMainPlayerController* PC = nullptr;
	
	// SUBCLASSES
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UInventoryScreenWidget> InventoryScreenHUDComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UInteractionTooltipWidget> InteractionTooltipHUDComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UScreenGameplayDebugWidget> ScreenGameplayDebugHUDComponent;
	
	
	// WIDGET INSTANCES
	UPROPERTY(BlueprintReadWrite);
	UInventoryScreenWidget* InventoryScreenWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	UInteractionTooltipWidget* InteractionTooltipWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	UScreenGameplayDebugWidget* ScreenGameplayDebugWidget = nullptr;
	
	
	UFUNCTION()
	void SetupHUD();
	
	UFUNCTION()
	void ToggleInventory();
	
	UFUNCTION()
	void HandleInteractionTooltip();
	
	UFUNCTION()
	void ShowInteractionTooltip(UInteractableComponent* inFocusedActor);
	
	UFUNCTION()
	void HideInteractionTooltip();
	
	UFUNCTION()
	void PassReferences();
	
	UFUNCTION()
	void UpdateInteractableTooltipLocation();
};


