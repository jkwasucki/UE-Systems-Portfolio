// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UInventorySlotTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetDataBP(const FText& ItemName, const FText& ItemDescription);
};
