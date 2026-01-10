// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "Types/ItemTypes.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"
class UInventoryComponent;
class UAttributesComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped2, const FItemData&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped2, const FItemData&, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, USkeletalMesh*, SkeletalMesh, EEquipmentType, EquipmentType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, EEquipmentType, EquipmentType);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();
	
protected:
	virtual void BeginPlay() override;

public:	
	//LINKS
	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;
	
	
	//PROPERTIES
	TMap<EEquipmentType,FName> Equipment;
	
	//API
	UFUNCTION()
	bool ResponseTryAddItem(FName ItemID,FName& OutReturnedEquippedItem);
	UFUNCTION()
	void RemoveItem(EEquipmentType Slot);
	
	// Methods - Inventory/Equipment
	UFUNCTION()
	void SwapWithInventory(EEquipmentType Slot, FName DroppedItemID, int32 InventorySlot);
	UFUNCTION()
	void AddFromInventory(EEquipmentType EquipmentSlot,  FName EquipmentItemID,int32 InventorySlot);
	UFUNCTION()
	void MoveToInventory(EEquipmentType ItemOnSlot);
	
	// Methods - other
	void SetInventoryComponentLink(UInventoryComponent* InventoryComponent);
	
	// API
	const FItemData* GetEquipmentDataBySlot(EEquipmentType Slot);
	bool IsEquippable(FName ItemID);	
	bool EquippmentTypeMatch(EEquipmentType EquipmentType, FName ItemID);
	bool IsOccupied(EEquipmentType Slot);
	FName GetItemID(EEquipmentType Slot);
	UTexture2D* GetItemTexture(EEquipmentType Slot);
	
	
	
	
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemsDataTable;
	
	
	FOnEquipmentChanged OnEquipmentChanged;
	FOnItemEquipped OnItemEquippedDelegate;
	FOnItemUnequipped OnItemUnequippedDelegate;
	FOnItemUnequipped2 OnItemUnequipped2Delegate;
	FOnItemEquipped2 OnItemEquipped2Delegate;
		
};

