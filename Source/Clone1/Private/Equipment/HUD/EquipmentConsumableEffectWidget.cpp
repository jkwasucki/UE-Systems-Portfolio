// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentConsumableEffectWidget.h"

void UEquipmentConsumableEffectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bStartTick)
	{
		float RemainingTime = EndTime - GetWorld()->GetTimeSeconds();
		RemainingTime = FMath::Max(RemainingTime, 0.f);

		ElapseTime(RemainingTime);

		if (RemainingTime <= 0.f)
		{
			bStartTick = false;
		}
	}
	
}

void UEquipmentConsumableEffectWidget::ElapseTime(float DeltaTime)
{
	int32 TotalSeconds = FMath::CeilToInt(DeltaTime);

	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	FText TimeText = FText::FromString(
		FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)
	);

	TimeTextWidget->SetText(TimeText);
}

void UEquipmentConsumableEffectWidget::SetData(UTexture2D* Icon, FText ItemName, float inDuration)
{
	NameTextWidget->SetText(ItemName);
	IconWidget->SetBrushFromTexture(Icon);
	
	
	UpdateRemainingTime(inDuration);
}

void UEquipmentConsumableEffectWidget::UpdateRemainingTime(float inDuration)
{
	Duration = inDuration;
	
	if (UWorld* World = GetWorld())
	{
		EndTime = World->GetTimeSeconds() + Duration;
		bStartTick = true;
	}
}
