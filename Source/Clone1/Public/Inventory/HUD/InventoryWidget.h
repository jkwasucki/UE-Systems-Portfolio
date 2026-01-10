// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContextMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlotSplitWidget.h"
#include "InventorySlotDraggedWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCarriedStack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;

	// Default constructor (inline)
	FCarriedStack() {}

	FCarriedStack(FName InItemID, int32 InAmount)
		: ItemID(InItemID)
		, Amount(InAmount)
	{}

	bool IsValid() const
	{
		return Amount > 0 && ItemID != NAME_None;
	}

	void Reset()
	{
		ItemID = NAME_None;
		Amount = 0;
	}
};


class UInventoryComponent;
class UInventoryScreenWidget; 
class UInventorySlotWidget;
class UInventoryDropAreaWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestScreenWidget_DragWidget, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestScreenWidget_ContextWidget, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestScreenWidget_SplitWidget, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddAmount, int32, Amount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestConsume, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestEquip, int32, SlotIdnex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPopulateIndex, int32, ToIndex, FCarriedStack, CarriedStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeIndex, int32, FromIndex, int32, ToIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSwapForward, int32, FromIndex, int32, ToIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMergeStacksForward, int32, FromIndex, int32, ToIndex, int32, AppendedAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMergeStacksFromSplit, int32, ToIndex, int32, AppendedAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestDecreaseAmount, int32, Index, int32, DecreaseAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbortSplitDrag, int32, SlotIndex, int32, RestoreChangeAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStackSplit, int32, SlotIndex, int32, NewBaseAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemDropped,int32, SlotIndex, int32, Amount, bool, bDrop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSplitDropped,FName, ItemID, int32, Amount);

UCLASS()
class CLONE1_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	bool bIsInitialized = false;
	
	//MAIN ORCHESTRATOR
	UPROPERTY()
	UInventoryScreenWidget* Orchestrator = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* InventoryComponent = nullptr;

	
	
	
	// SPLITTING
	int32 CurrentSplittingSlotIndex;
	UPROPERTY(BlueprintReadOnly)
	bool IsSplitting = false;
	
	// CONTEXT MENU
	bool bIsContextMenuOpen = false;
	
	
	
	//CARRYING (ARTIFICIAL DRAG)
	int32 CarryFromSlotIndex;
	UPROPERTY(BlueprintReadOnly, Category = "Carry")
	bool bIsCarryingStack = false;
	UPROPERTY(BlueprintReadOnly, Category = "Carry")
	FCarriedStack CarriedStack;
	UFUNCTION()
	void BeginCarryFromSplit(int32 NewStackAmount, int32 Index); 
	UFUNCTION()
	void BeginCarry(const FItemStack& InStack);
	UFUNCTION()
	void EndCarry(bool IsSuccess); 
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UUserWidget> SlotWidget;
	
	
	
	
	
	
	// DELEGATES
	FOnItemDropped OnItemDroppedDelegate;
	FOnStackSplit OnStackSplitDelegate;
	FOnAbortSplitDrag OnAbortSplitDragDelegate;
	FOnMergeStacksForward OnMergeStacksForwardDelegate; 
	FOnSwapForward OnSwapForwardDelegate;
	FOnChangeIndex OnChangeIndexDelegate;
	FOnPopulateIndex OnPopulateIndexDelegate;
	FOnItemSplitDropped OnItemSplitDroppedDelegate;
	FOnRequestEquip OnRequestEquipDelegate;
	FOnRequestScreenWidget_SplitWidget OnRequestScreenWidget_SplitWidget;
	FOnRequestScreenWidget_ContextWidget OnRequestScreenWidget_ContextWidget;
	FOnRequestScreenWidget_DragWidget OnRequestScreenWidget_DragWidget;
	FOnAddAmount OnAddAmountDelegate;
	FOnMergeStacksFromSplit OnMergeStacksFromSplitDelegate;
	FOnRequestDecreaseAmount OnRequestDecreaseAmountDelegate;
	FOnRequestConsume OnRequestConsumeDelegate;
	
	// Cached slots
	UPROPERTY()
	TArray<UInventorySlotWidget*> SlotWidgets;
	
	// BINDINGS
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGrid;
	UPROPERTY(meta=(BindWidget))
	UInventoryDropAreaWidget* BP_DropAreaWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTextBlock* WeightText;

	
	
	
	UFUNCTION()
	void Init(UInventoryScreenWidget* Widget);
	UFUNCTION()
	void GenerateSlots();
	UFUNCTION()
	void UpdateSlots();
	UFUNCTION()
	void UpdateWeight();
	UFUNCTION()
	void NotifyItemDropped(int32 SlotIndex,int32 Amount, bool bFromSplit);

	
	
	UFUNCTION()
	bool IsSlotOccupied(int32 SlotIndex);
	
	// GETTERS FROM SOURCE
	UFUNCTION(BlueprintPure)
	UTexture2D* GetItemIcon(int32 SlotIndex);
	UFUNCTION(BlueprintPure)
	const int32 GetStackAmount(int32 SlotIndex);
	UFUNCTION(BlueprintPure)
	FName GetItemID(int32 SlotIndex);
	UFUNCTION()
	int32 GetMaxStack(int32 SlotIndex);
	UFUNCTION()
	FText GetItemName(int32 SlotIndex);
	UFUNCTION()
	FText GetItemDescription(int32 SlotIndex);
	
	UFUNCTION()
	void RequestMergeStacks(int32 FromIndex, int32 ToIndex, int32 AppendedAmount);
	UFUNCTION()
	void RequestMergeStacksFromSplit(int32 ToIndex, int32 Amount);
	UFUNCTION()
	void RequestSwap(int32 FromIndex, int32 ToIndex);
	UFUNCTION()
	void RequestSwapFromSplit(int ToIndex);
	UFUNCTION()
	void RequestChangeIndex(int32 FromIndex, int32 ToIndex,bool bFromSplit);
	UFUNCTION()
	void ContextMenu_RequestEquip(int32 SlotIndex);
	UFUNCTION()
	void OnModalsDisabled();

	UFUNCTION()
	void HandleRightClickGLOBAL();
	UFUNCTION()
	void HandleRightClickOnSlot(int32 SlotIndex);
	
	// SPLITTING
	UFUNCTION()
	void StartSplitting(int32 SlotIndex);
	UFUNCTION()
	void StopSplitting(bool bAbort = true);
	
	// CONTEXT MENU WIDGET
	UFUNCTION()
	void SetupContextMenu(UInventoryContextMenuWidget* ContextMenuWidget);
	UFUNCTION()
	void SetupSplitWidget(UInventorySlotSplitWidget* SplitWidget);
	UFUNCTION()
	void RequestContextMenu(bool bIsEquippable, bool bIsSplittable, bool bIsConsumable, int32 SlotIndex);
	UFUNCTION()
	void OnContextMenuDisabled();
	UFUNCTION()
	void Consume(int32 SlotIndex);
	
	
	
	UFUNCTION()
	void Enable();
	UFUNCTION()
	void Disable();
	
};


