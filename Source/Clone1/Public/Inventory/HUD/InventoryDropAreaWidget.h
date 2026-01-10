// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FItemStack.h"

#include "Blueprint/UserWidget.h"
#include "InventoryDropAreaWidget.generated.h"

/**
 * 
 */
class UInventoryWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDropDelegate, int32, SlotIndex, int32, Amount, bool, bFromSplit);
UCLASS()
class CLONE1_API UInventoryDropAreaWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UInventoryWidget* InventoryWidgetRef;
	UPROPERTY(BlueprintCallable)
	FOnDropDelegate OnDropDelegate;
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
