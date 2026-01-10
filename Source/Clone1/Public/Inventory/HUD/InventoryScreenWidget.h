
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotSplitWidget.h"
#include "InventoryContextMenuWidget.h"
#include "InventorySlotDraggedWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "InventoryScreenWidget.generated.h"

/**
 * 
 */
class AMainPlayerController;
class UInventoryWidget;
class UEquipmentAttributesWidget;
class UEquipmentWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScreenOnRightClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModalsDisabled);


UCLASS()
class CLONE1_API UInventoryScreenWidget : public UUserWidget // MAIN HUD ORCHESTRATOR FOR INVENTORY + EQUIPMENT WIDGETS

{
	GENERATED_BODY()

protected:
	bool bIsActive = false;
	bool bPopupLayerActive  = false;
	
	bool bIsContexWidgetActive = false;
	bool bIsSplitWidgetActive = false;
	bool bIsDragWidgetActive = false;
	
	UPROPERTY()
	FVector2D PrevMousePos;
	
	UFUNCTION()
	void DisableModals();
	
	
	//PENDING
	UPROPERTY()
	UInventoryContextMenuWidget* PendingContextWidget = nullptr;
	
public:
	UPROPERTY()
	AMainPlayerController* PC = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UInventoryWidget* InventoryWidget;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UEquipmentWidget* EquipmentWidget;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UEquipmentAttributesWidget* AttributesWidget;
	
	
	// SUBCLASSES
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DraggedWidgetComponent;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryContextMenuWidget> InventoryContextMenuComponent;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotSplitWidget>InventoryItemSplitWidgetComponent;
	
	// WIDGET BINDINGS
	UPROPERTY(meta=(BindWidget))
	UButton* PopupLayer;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* PopupCanvas;
	
	// WIDGET INSTANCES
	UPROPERTY()
	UInventoryContextMenuWidget* InventoryContextMenuInstance;
	UPROPERTY()
	UInventorySlotDraggedWidget* DraggedWidgetInstance;
	UPROPERTY()
	UInventorySlotSplitWidget* InventorySplitWidget;
	
	// DELEGATES
	FScreenOnRightClick OnRightClickDelegate;
	FOnModalsDisabled OnModalsDisabledDelegate;
	
	
	
	
	UFUNCTION()
	void Toggle();
	UFUNCTION()
	void Init(AMainPlayerController* PlayerController);
	UFUNCTION()
	void PassRightClickEvent();
	UFUNCTION()
	void ToggleSplitWidget(bool bState);
	UFUNCTION()
	void ToggleContextWidget(bool bState);
	UFUNCTION()
	void ToggleDragWidget(bool bState);
	UFUNCTION()
	void OnPopupLayerClick();
	UFUNCTION()
	void PositionWidgetAtCursor(UWidget* Widget);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

};

