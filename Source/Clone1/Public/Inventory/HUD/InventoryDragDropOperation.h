// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly)
	int32 FromSlotIndex;
	UPROPERTY(BlueprintReadOnly)
	int32 FromAmount;
	UPROPERTY(BlueprintReadOnly)
	FName ItemID;
	
	
	UFUNCTION(BlueprintCallable)
	void SetDragDropOperation(int32 inFromSlotIndex, int32 infromAmount, FName inItemID);
};
