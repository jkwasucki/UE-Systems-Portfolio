// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "EquipmentConsumableEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UEquipmentConsumableEffectWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	bool bStartTick = false;
	
	float Duration = 0.f;
	float EndTime = 0.f;
	
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void ElapseTime(float DeltaTime);
	
public:
	
	UPROPERTY(meta=(BindWidget))
	UImage* IconWidget;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameTextWidget;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimeTextWidget;
	
	UFUNCTION()
	void SetData(UTexture2D* Icon, FText ItemName, float Duration);
	UFUNCTION()
	void UpdateRemainingTime(float Duration);
};
