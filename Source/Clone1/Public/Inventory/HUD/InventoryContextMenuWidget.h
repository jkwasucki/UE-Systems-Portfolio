// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/UITypes.h"
#include "Components/SizeBox.h"
#include "InventoryContextMenuWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquip, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplit ,int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsume, int32, SlotIndex);

UCLASS()
class CLONE1_API UInventoryContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY()
	int32 ContextSlotIndex;
	
	void ToggleButton(bool bState,EInventoryContextAction Context);
	void SetupButtons();
	UFUNCTION()
	void EquipPress();
	UFUNCTION()
	void SplitPress();
	UFUNCTION()
	void ConsumePress();
	
public:
	
	UPROPERTY()
	bool bIsActive = false;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* EquipBox;
	UPROPERTY(meta=(BindWidget))
	USizeBox* SplitBox;
	UPROPERTY(meta=(BindWidget))
	USizeBox* ConsumeBox;
	
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Toggle(bool bState, bool bCanEquip, bool bCanSplit, bool bCanConsume,int32 SlotIndex);
	
	FOnConsume OnConsumeDelegate;
	FOnSplit OnSplitDelegate;
	FOnEquip OnEquipDelegate;	
	

	
};
