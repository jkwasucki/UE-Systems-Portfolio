// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentComponent.h"
#include "Types/ItemTypes.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */

class UInventoryScreenWidget;
class UEquipmentSlotWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentRequestEquip, EEquipmentType, Slot);



UCLASS()
class CLONE1_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool bIsInitialized = false;
	
	UPROPERTY()
	UInventoryScreenWidget* Orchestrator = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UEquipmentComponent* EquipmentComponent = nullptr;	
	
	
	UPROPERTY(BlueprintReadOnly)
	TMap<EEquipmentType, UEquipmentSlotWidget*> EquipmentSlots;
	
	FOnRequestDrop OnRequestDropDelegate;
	
	UFUNCTION()
	void Init(UInventoryScreenWidget* inOrchestrator);
		
	UFUNCTION()
	void Enable();
	UFUNCTION()
	void Disable();
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void RefreshEquipment();
	

	
};
