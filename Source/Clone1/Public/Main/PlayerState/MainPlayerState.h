// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributesComponent.h"
#include "Equipment/EquipmentComponent.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMainPlayerState();
	
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UEquipmentComponent* EquipmentComponent = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAttributesComponent* AttributesComponent = nullptr;
};
