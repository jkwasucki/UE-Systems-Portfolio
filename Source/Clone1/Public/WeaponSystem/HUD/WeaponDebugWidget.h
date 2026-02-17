// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/FWeaponInstanceRuntimeSnapshot.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "WeaponDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UWeaponDebugWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void UpdateWidget(FWeaponInstanceRuntimeSnapshot Snapshot);
	UFUNCTION()
	void UpdateRuntimeData(FWeaponInstanceRuntimeSnapshot& Snapshot);
	UFUNCTION()
	void UpdateStaticData(FWeaponInstanceRuntimeSnapshot& Snapshot);
	UPROPERTY()
	bool bIsStaticBoxDisplayed = false;
public:
	
	UFUNCTION()
	void Init(APlayerController* InPC);
	
	// RUNTIME
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InstanceID;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* State;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FireMode;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HasOwner;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HasDefinition;
	
	// STATIC
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* StaticData;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Name;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TBSValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SpreadValue;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FireModes;
};