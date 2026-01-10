// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentWidget.h"
#include "Inventory/HUD/InventoryScreenWidget.h"
#include "Inventory/HUD/InventorySlotTooltipWidget.h"
#include "Types/ItemTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EquipmentSlotWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentSlotOnRightClick, EEquipmentType, EquipmentType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDragDrop, EEquipmentType, Slot, FName, ItemID, int32, FromInventoryIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwapWithInventory, EEquipmentType, Slot, FName, ItemID, int32, FromInventoryIndex);

UCLASS()
class CLONE1_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied = false;
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	
	UPROPERTY(BlueprintReadOnly)
	UEquipmentWidget* EquipmentWidget = nullptr;
	
	// SUBCLASS WIDGET
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotTooltipWidget> SlotTooltipWidget;
	// WIDGET INSTANCE
	UPROPERTY()
	UInventorySlotTooltipWidget* CachedTooltip = nullptr;
	
	 UFUNCTION(BlueprintCallable)
	FName EQGetItemID()
	 {
		 return EquipmentWidget->EquipmentComponent->GetItemID(EquipmentType);
	 }
	
	UFUNCTION(BlueprintCallable)
	UTexture2D* EQ_GetItemIcon()
	 {
		 return EquipmentWidget->EquipmentComponent->GetItemTexture(EquipmentType);
	 }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	EEquipmentType EquipmentType;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetDataBP(const FItemData& ItemData);
	UFUNCTION()
	void SetData(const FItemData& ItemData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CleanupBP();
	UFUNCTION()
	void Cleanup();
	
	UFUNCTION()
	void UpdateTooltip();
	
	
	UFUNCTION()
	void ResolveOnDrop(int32 FromSlot, FName ItemID);
	
	
	FOnDragDrop OnDroppedOnto_FromInventory;
	FEquipmentSlotOnRightClick OnRightClickDelegate;
	FOnSwapWithInventory OnSwapWithInventoryDelegate;
	
	
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
