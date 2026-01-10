// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/AttributeTypes.h"
#include "Structs/AttributeValue.h"
#include "Inventory/InventoryComponent.h"
#include "AttributesComponent.generated.h"

class UEquipmentComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributesChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConsumableEffectAppear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableEffectExtend, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableEffectEnd, FName, ItemID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	
	UPROPERTY(EditAnywhere)
	TMap<EAttribute,FAttributeValue> Attributes;
	
	UPROPERTY()
	TMap<FName, FConsumableEffect> ConsumableEffects;
	UPROPERTY()
	TMap<FName, FTimerHandle> EffectTimers;
	
	UFUNCTION()
	void RemoveModifiers(const FItemData& ItemData);

	UFUNCTION()
	void AddModifiers(const FItemData& ItemData);
public:	
	const float MaxSpeedAttribute = 100.f;
	const float MaxHealthAttribute = 100.f;
	const float MaxArmorAttribute = 100.f;
	
	FOnAttributesChanged OnAttributesChangedDelegate;
	FOnConsumableEffectAppear OnConsumableEffectAppearDelegate;
	FOnConsumableEffectEnd OnConsumableEffectEndDelegate;
	FOnConsumableEffectExtend OnConsumableEffectExtendDelegate;
	// API
	UFUNCTION()
	float GetFinalAttributeValue(EAttribute Attribute) const;
	UFUNCTION()
	float GetBaseAttributeValue(EAttribute Attribute) const;
	UFUNCTION()
	float GetBonusAttributeValue(EAttribute Attribute) const;
	UFUNCTION()
	const FItemBaseData& GetBaseItemData(FName ItemID);
	UFUNCTION()
	bool IsAnyBuffActiveForAttribute(EAttribute Attribute);
	UFUNCTION()
	float GetDurationForEffect(FName ItemID);
	
	UFUNCTION()
	void OnConsumableUsed(FName ItemID);
	UFUNCTION()
	void ApplyConsumableEffect(FName ItemID, const FConsumableEffect& Effect);
	UFUNCTION()
	void ResetConsumableEffect(FName ItemID, const FConsumableEffect& Effect);
	UFUNCTION()
	void RemoveConsumableEffect(FName ItemID);
	UFUNCTION()
	const TMap<FName, FConsumableEffect>& GetConsumableEffects();	
	
	UPROPERTY(EditAnywhere)
	UDataTable* ItemsDataTable;
	
	void SetEquipmentComponentLink(UEquipmentComponent* EquipmentComponent);
	void SetInventoryComponentLink(UInventoryComponent* InventoryComponent);
};
