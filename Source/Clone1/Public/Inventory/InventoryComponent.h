// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FItemStack.h"
#include "Structs/FItemData.h"
#include "InventoryComponent.generated.h"  




class UEquipmentComponent;
class AMainPlayerState;


// DELEGATES
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRejected, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestDrop, FItemStack, ItemStack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeightChange, int32, CurrentWeight, int32, MaxWeight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemConsumed,AActor*, Owner, FCharacterEffect&, Effect, FGuid, SourceInstanceID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventoryComponent();

protected:
	
	virtual void BeginPlay() override;
	

public:	
	
	// Connections
	UPROPERTY()
	UEquipmentComponent* EquipmentComponent = nullptr;
	
	//DELEGATES
	FOnInventoryChanged OnInventoryChanged;
	FOnRequestDrop OnRequestDropDelegate;
	FOnWeightChange OnWeightChangeDelegate;
	FOnItemRejected OnItemRejectedDelegate;
	
	FItemStack LastRemovedStack; 
	FOnItemConsumed OnItemConsumedDelegate;
	
	
	
	
	
	//PROPERTIES
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Inventory")
	int32 Capacity = 12;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Inventory")
	int32 MaxWeight = 130;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Inventory")
	int CurrentWeight = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Inventory")
	TArray<FItemStack> Items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemsDataTable = nullptr;
	
	//API 
	const FItemData* GetItemData(FName ItemID) const;
	const FItemData* GetItemDataByIndex(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FItemStack>& GetItems();
	const FItemStack* GetStackByIndex(int32 Index);
	
	
	
	// FUNCTIONALITY
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(int32 Index, int32 Amount, bool bDrop);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromSplit(FName ItemID, int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemID, int32 Amount);	
	UFUNCTION()
	void AddItemToIndex(FName ItemID, int32 Amount, int32 InventorySlot);
	
	UFUNCTION()
	void SplitStack(int32 Index, int32 AmountChange);
	UFUNCTION()
	void RestoreStack(int32 Index, int32 RestoreAmount);
	UFUNCTION()
	void MergeStacks(int32 FromIndex, int32 ToIndex, int32 AppendedAmount);
	UFUNCTION()
	void SwapStacks(int32 FromIndex, int32 ToIndex);
	UFUNCTION()
	void ChangeIndex(int32 FromIndex, int32 ToIndex);
	UFUNCTION()
	void PopulateIndex(int32 ToIndex, FCarriedStack CarriedStack);
	UFUNCTION()
	void SwapWithEquipment(int32 InventorySlot);
	UFUNCTION()
	void AddFromEquipment(EEquipmentType EquipmentSlot, int32 InventorySlot, FName EquipmentItemID);
	UFUNCTION()
	void UpdateWeight();
	UFUNCTION()
	void AppendAmount(int32 Index, int32 Amount);
	UFUNCTION()
	void DecreaseAmount(int32 Index, int32 Amount);
	UFUNCTION()
	void ConsumeItem(int32 Index);

	void SetEquipmentComponentLink(UEquipmentComponent* inEquipmentComponent);
	
private:
	bool HasSpace()
	{
		for (FItemStack Item : Items)
		{
			if (Item.ItemID == NAME_None)
			{
				return true;
			}
		}
		return false;
	}
};
