// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotSplitWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplitWidgetDismiss, bool, bAbort);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSplitWidgetConfirm, int32, NewStackAmount, int32, FromSlot);
UCLASS()
class CLONE1_API UInventorySlotSplitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetData(int32 StackAmount, UTexture2D* Icon);
	
	UPROPERTY(BlueprintCallable)
	FOnSplitWidgetDismiss OnDismissDelegate;
	UPROPERTY(BlueprintCallable)
	FOnSplitWidgetConfirm OnConfirmDelegate;
};
