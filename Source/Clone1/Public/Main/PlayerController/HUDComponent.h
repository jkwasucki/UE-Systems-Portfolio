// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDefinition.h"
#include "OtherHUD/GameplayDebug/ScreenGameplayDebugWidget.h"
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
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentInteractable = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UInteractionDefinition> InteractionDefinition;
	
public:	
	UPROPERTY()
	TWeakObjectPtr<AMainPlayerController> PC;

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
	

public:
	UHUDComponent();
	
	UFUNCTION()
	void Init(AMainPlayerController* InPC);
	
	UFUNCTION()
	void OnNewInteractable(UInteractionDefinition* Definition,AActor* Interactable);
	UFUNCTION()
	void OnToggleInventory();
	
	UFUNCTION()
	void SetupHUD();
	
	UFUNCTION()
	void HandleInteractionTooltip();
	
	UFUNCTION()
	void ShowInteractionTooltip(AActor* inFocusedActor);
	
	UFUNCTION()
	void HideInteractionTooltip();
	
	UFUNCTION()
	void PassReferences();
	
	UFUNCTION()
	void UpdateInteractableTooltipLocation();
	
};


