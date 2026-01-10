// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h" 
#include "Structs/FItemData.h"
#include "Structs/FItemStack.h"
#include "InventorySlotTooltipWidget.h"
#include "InventoryWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "InventorySlotWidget.generated.h"


/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotOnRightClick, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwapEquipmentItems,int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMoveOntoFromEQ,EEquipmentType ,EquipmentSlot,int32, ToIndex, FName, DroppedItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSwap, int32, FromIndex, int32, ToIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwapFromSplit,int32, ToIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMoveOnto, int32, FromIndex, int32, ToIndex, bool, FromSplit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMergeStacks, int32, FromIndex, int32, ToIndex, int32, AppendedAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMergeStacksFSplit, int32, ToIndex, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleSplit, int32, SlotIndex);
UCLASS()
class CLONE1_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
private:

public:
	// Reference to the parent
	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* InventoryWidget = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;
	
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UOverlay* ContentBox;
	UPROPERTY(meta=(BindWidget))
	UButton* Button;
	UPROPERTY(meta=(BindWidget))
	UImage* ArtificialHover;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventorySlotTooltipWidget> SlotTooltipWidget;
	UInventorySlotTooltipWidget* CachedTooltip = nullptr;
	
	UFUNCTION(BlueprintPure)
	bool IsOccupied() const
	{
		return InventoryWidget->IsSlotOccupied(SlotIndex);
	}
	
	
	//DELEGATES
	UPROPERTY(BlueprintCallable)
	FOnToggleSplit OnToggleSplitDelegate;
	FOnMergeStacks OnMergeStacksDelegate;
	FOnMergeStacksFSplit OnMergeStacksFromSplitDelegate;
	
	FOnSwap OnSwapDelegate;
	FOnSwapFromSplit OnSwapFromSplitDelegate;
	
	FOnMoveOnto OnMoveOntoDelegate;
	FInventorySlotOnRightClick OnRightClickDelegate;
	FOnMoveOntoFromEQ OnMoveOntoFromEquipmentDelegate;
	FOnSwapEquipmentItems OnSwapEquipmentItemsDelegate;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetDataBP(const FItemStack& ItemStack, const FItemData& ItemData );
	UFUNCTION()
	void SetData(const FItemStack& ItemStack, const FItemData& ItemData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CleanupBP();
	UFUNCTION()
	void Cleanup();
	
	UFUNCTION()
	void ResolveDropOnto(int32 FromSlot, FName ItemID, int32 Amount, bool bFromSplit);
	UFUNCTION()
	void ResolveDropOnto_FromEquipment(EEquipmentType FromSlot, FName& ItemID);
	
	UFUNCTION()
	void UpdateTooltip();
	
};

