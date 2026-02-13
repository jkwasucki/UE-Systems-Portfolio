// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/AttributeTypes.h"
#include "Structs/FAttributeValue.h"
#include "Inventory/InventoryComponent.h"
#include "AttributesComponent.generated.h"

class UEffectsComponent;
class UEquipmentComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributesChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EAttribute,FFAttributeValue> Attributes;

public:
	UPROPERTY()
	TWeakObjectPtr<UEffectsComponent> EffectsComponent = nullptr;
	
	const float MaxSpeedAttribute = 100.f;
	const float MaxHealthAttribute = 100.f;
	const float MaxArmorAttribute = 100.f;
	
	FOnAttributesChanged OnAttributesChangedDelegate;
	
	UPROPERTY(EditAnywhere)
	UDataTable* ItemsDataTable;

public:
	UAttributesComponent();

	void SetEquipmentComponentLink(UEquipmentComponent* EquipmentComponent);
	void SetEffectsComponentLink(UEffectsComponent* EffectsComponent);
	UFUNCTION()
	void IncreaseAttribute(EAttribute Attribute, float Value);
	UFUNCTION()
	void DecreaseAttribute(EAttribute Attribute, float Value);

	UFUNCTION()
	void AddModifiers(const FItemData& ItemData);
	UFUNCTION()
	void RemoveModifiers(const FItemData& ItemData);
	
	// GETTERS
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
	const TMap<EAttribute,FFAttributeValue>& GetAttributes();
};
