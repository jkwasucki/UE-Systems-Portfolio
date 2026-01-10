// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentAttributesWidget.h"

#include "Main/PlayerController/MainPlayerController.h"
#include "Main/PlayerState/MainPlayerState.h"


void UEquipmentAttributesWidget::Init(UInventoryScreenWidget* inOrchestrator)
{
	if (bIsInitialized) return;
	
	bIsInitialized = true;
	
	if (inOrchestrator)
	{
		if (inOrchestrator->PC)
		{
			if (AMainPlayerState* PS = inOrchestrator->PC->GetPlayerState<AMainPlayerState>())
			{
				AttributesComponent = PS->AttributesComponent;
				AttributesComponent->OnAttributesChangedDelegate.AddDynamic(this, &UEquipmentAttributesWidget::OnGetNotified);
				AttributesComponent->OnConsumableEffectAppearDelegate.AddDynamic(this, &UEquipmentAttributesWidget::DisplayConsumableEffectWidget);
				AttributesComponent->OnConsumableEffectEndDelegate.AddDynamic(this, &UEquipmentAttributesWidget::RemoveConsumableEffectWidget);
				AttributesComponent->OnConsumableEffectExtendDelegate.AddDynamic(this, &UEquipmentAttributesWidget::OnEffectExtended);
			}
		}
	}
}

void UEquipmentAttributesWidget::Toggle()
{
	UpdateAttributeHUD(EAttribute::Armor);
	UpdateAttributeHUD(EAttribute::Health);
	UpdateAttributeHUD(EAttribute::Speed);
}

void UEquipmentAttributesWidget::DisplayConsumableEffectWidget()
{
	const TMap<FName, FConsumableEffect>& Effects = AttributesComponent->GetConsumableEffects();
	
	if (Effects.Num() <= 0) return;
	for (const TPair<FName, FConsumableEffect>& Pair : Effects)
	{
		GenerateConsumableEffectWidget(Pair.Key, Pair.Value.Duration);
	}
	HandleBuffTextColor();	
}

void UEquipmentAttributesWidget::GenerateConsumableEffectWidget(FName ItemID, float Duration)
{
	if (ConsumableEffectWidgetComponent)
	{
		UEquipmentConsumableEffectWidget* ConsumableWidget = CreateWidget<UEquipmentConsumableEffectWidget>(GetOwningPlayer(),ConsumableEffectWidgetComponent);
		if (ConsumableWidget)
		{
			const FItemBaseData& BaseItemData = AttributesComponent->GetBaseItemData(ItemID);
			ConsumableWidget->SetData(BaseItemData.ItemIcon, BaseItemData.ItemName,Duration);
			ConsumableEffectWidgetMap.Add(ItemID, ConsumableWidget);
			
			ConsumableEffects->AddChild(ConsumableWidget);
		}
	}
}

void UEquipmentAttributesWidget::RemoveConsumableEffectWidget(FName ItemID)
{
	UE_LOG(LogTemp,Warning, TEXT("REMOVE EFFECT"));
	for (TPair<FName, UEquipmentConsumableEffectWidget*> Pair : ConsumableEffectWidgetMap)
	{
		if (Pair.Key == ItemID)
		{
			Pair.Value->RemoveFromParent();
			ConsumableEffectWidgetMap.Remove(Pair.Key);
			HandleBuffTextColor();	
			return;
		}
	}
	
}

void UEquipmentAttributesWidget::HandleBuffTextColor()
{

	if (AttributesComponent->IsAnyBuffActiveForAttribute(EAttribute::Speed))
	{
		
		TotalSpeedValue->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		TotalSpeedValue->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	if (AttributesComponent->IsAnyBuffActiveForAttribute(EAttribute::Armor))
	{
		TotalArmorValue->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		TotalArmorValue->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	if (AttributesComponent->IsAnyBuffActiveForAttribute(EAttribute::Health))
	{
		TotalHealthValue->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		TotalHealthValue->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
}

void UEquipmentAttributesWidget::OnEffectExtended(FName ItemID)
{
	for (TPair<FName, UEquipmentConsumableEffectWidget*> Pair : ConsumableEffectWidgetMap)
	{
		if (Pair.Key == ItemID)
		{
			float dur = AttributesComponent->GetDurationForEffect(ItemID);
			if (dur > 0.0f)
			{
				Pair.Value->UpdateRemainingTime(dur);
				return;
			}
		}
	}
}


void UEquipmentAttributesWidget::OnGetNotified()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green,TEXT("OnGetNotified()"));
	UpdateAttributeHUD(EAttribute::Armor);
	UpdateAttributeHUD(EAttribute::Health);
	UpdateAttributeHUD(EAttribute::Speed);
}



void UEquipmentAttributesWidget::UpdateAttributeHUD(EAttribute Attribute)
{
	switch (Attribute)
	{
		case EAttribute::Armor:
			{
				float TotalV = AttributesComponent->GetFinalAttributeValue(Attribute);
				float BonusV = AttributesComponent->GetBonusAttributeValue(Attribute);
				UE_LOG(LogTemp,Warning,TEXT("ARMOR: %f"), TotalV);
				TotalArmorValue->SetText(FText::AsNumber(FMath::Abs(TotalV)));
				BonusArmorValue->SetText(FormatBonusText(BonusV));	
				BonusArmorValue->SetColorAndOpacity(GetBonusColor(BonusV));
				break;
			}
		case EAttribute::Health:
			{
				float TotalV1 = AttributesComponent->GetFinalAttributeValue(Attribute);
				float BonusV1 = AttributesComponent->GetBonusAttributeValue(Attribute);
				TotalHealthValue->SetText(FText::AsNumber(FMath::Abs(TotalV1)));
				BonusHealthValue->SetText(FormatBonusText(BonusV1));	
				BonusHealthValue->SetColorAndOpacity(GetBonusColor(BonusV1));
				break;
			}
		case EAttribute::Speed:
			{
				float TotalV2 = AttributesComponent->GetFinalAttributeValue(Attribute);
				float BonusV2 = AttributesComponent->GetBonusAttributeValue(Attribute);
				TotalSpeedValue->SetText(FText::AsNumber(FMath::Abs(TotalV2)));
				BonusSpeedValue->SetText(FormatBonusText(BonusV2));	
				BonusSpeedValue->SetColorAndOpacity(GetBonusColor(BonusV2));
				break;
			}
		
	}
}



// Utilities

FText UEquipmentAttributesWidget::FormatBonusText(float BonusValue)
{
	const int32 AbsVal = FMath::Abs(FMath::RoundToInt(BonusValue));

	if (BonusValue > 0.f)
	{
		return FText::Format(
			FText::FromString("(+{0})"),
			FText::AsNumber(AbsVal)
		);
	}
	else if (BonusValue < 0.f)
	{
		return FText::Format(
			FText::FromString("(-{0})"),
			FText::AsNumber(AbsVal)
		);
	}

	return FText::GetEmpty(); // or "(0)"
}

FSlateColor UEquipmentAttributesWidget::GetBonusColor(float BonusValue)
{
	if (BonusValue > 0.f)
	{
		return FSlateColor(FLinearColor::Green);
	}
	else if (BonusValue < 0.f)
	{
		return FSlateColor(FLinearColor::Red);
	}

	return FSlateColor(FLinearColor::Transparent);
}