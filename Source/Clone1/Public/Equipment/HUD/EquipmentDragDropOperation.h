// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/ItemTypes.h"
#include "Blueprint/DragDropOperation.h"
#include "EquipmentDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UEquipmentDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	EEquipmentType FromSlot;
	
	UPROPERTY(BlueprintReadWrite)
	int32 ToSlot;
	
	UPROPERTY(BlueprintReadWrite)
	FName ItemID;
	
	UFUNCTION(BlueprintCallable)
	void SetDragDropData(EEquipmentType  inFromSlot,FName inItemID);
	
};
