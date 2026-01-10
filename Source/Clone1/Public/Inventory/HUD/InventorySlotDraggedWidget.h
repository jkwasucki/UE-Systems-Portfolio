// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotDraggedWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UInventorySlotDraggedWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDragVisuals(UTexture2D* Texture2D, int32 StackAmount);
};
