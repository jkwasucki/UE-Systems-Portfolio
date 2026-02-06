// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HorText2Val.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "GameplayDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UGameplayDebugWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	TMap<FName, UTextBlock*> DisplayedEffectsOnPlayer;
	void SetTimer(FTimerHandle& Handle, float Duration, FTimerDelegate FallbackDelegate);
public:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	FTimerHandle AbilityBoxVisibilityTimer;
	
	
	
	UPROPERTY(meta=(BindWidgetOptional))
	USizeBox* AbilityBox;
	UPROPERTY(meta=(BindWidgetOptional))
	UVerticalBox* AbilityEffectsList;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* EffectsOnSelfList;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EntityType;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EntityState;
	
	
	//RES
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EnergyValue;
	
	//ATTR
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ArmorValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SpeedValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AttackValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthAttributeValue;
	
	//AB
	UPROPERTY(meta=(BindWidgetOptional))
    UTextBlock* AnimationLockMValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* AnimationModeValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* AbilityNameValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* AbilityModeValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* CooldownRemainingValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* CooldownDurationValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* CostValue;
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* TargetingValue;

	
	
	UFUNCTION()
	void UpdateResourcesData(FResourceDebugSnapshot& Snapshot);
	UFUNCTION()
	void UpdateAttributeData(FAttributeDebugSnapshot& Snapshot);
	
	UFUNCTION()
	void UpdateAll(FEntityGameplayDebugSnapshot& Snapshot);
	UFUNCTION()
	void UpdateState(EEntityState State);
	UFUNCTION()
	void ToggleAbilityBox(bool bState);
	
	UFUNCTION()
	void UpdateAbilityData(FAbilityDebugSnapshot& Snapshot);
	UFUNCTION()
	UTextBlock* SpawnEffectDescriptor(FText& Text,UVerticalBox* Box);
	UFUNCTION()
	void ShowEffects_OnSelf(FAbilityDebugSnapshot& Ability);
	UFUNCTION()
	void ShowEffects_OnAbility(FAbilityDebugSnapshot& Ability);
	UFUNCTION()
	void HandleDisplayEffect(FAbilityDebugSnapshot& Ability);
	UFUNCTION()
	void HideEffect_OnSelf(FAbilityDebugSnapshot& Ability);
	UFUNCTION()
	void ClearEffectsOnSelf();
	UFUNCTION()
	FText FormatEffectDescriptor(FCharacterEffect&Effect);
	
};
