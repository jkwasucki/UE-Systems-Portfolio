// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character//AttributesComponent.h"
#include "EquipmentConsumableEffectWidget.h"
#include "Blueprint/UserWidget.h"
#include "Structs/FItemData.h"
#include "Inventory/HUD/InventoryScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Main/Character/EffectsComponent.h"
#include "EquipmentAttributesWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UEquipmentAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void UpdateAttributeHUD(EAttribute Attribute);
	
	
	UPROPERTY()
	bool bIsInitialized = false;
	
	UPROPERTY()
	TMap<FGuid, UEquipmentConsumableEffectWidget*> ConsumableEffectWidgetMap;
	
public:
	UPROPERTY()
	UAttributesComponent* AttributesComponent = nullptr;
	UPROPERTY()
	UEffectsComponent* EffectsComponent = nullptr;
	// BIND WIDGETS
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalArmorValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalHealthValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TotalSpeedValue;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BonusArmorValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BonusHealthValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BonusSpeedValue;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ConsumableEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UEquipmentConsumableEffectWidget> ConsumableEffectWidgetComponent;
	
	
	// UTIL METCHODS
	FSlateColor GetBonusColor(float BonusValue);
	static FText FormatBonusText(float BonusValue);
	UFUNCTION()
	void HandleBuffTextColor();
	
	
	UFUNCTION()
	void OnGetNotified();
	UFUNCTION()
	void Init(UInventoryScreenWidget* inOrchestrator);
	UFUNCTION()
	void Toggle();
	UFUNCTION()
	void DisplayConsumableEffectWidget(UActiveEffectInstance* Effect);
	UFUNCTION()
	void GenerateConsumableEffectWidget(FGuid EffecInstanceID,float Duration);
	UFUNCTION()
	void RemoveConsumableEffectWidget(UActiveEffectInstance* Effect);
	
	UFUNCTION()
	void OnEffectExtended(UActiveEffectInstance* Effect);
};
