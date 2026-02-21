// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/HUD/EquipmentAttributesWidget.h"
#include "Main/Character/AttributesComponent.h"
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
			if (AMainPlayerController* PC = inOrchestrator->PC)
			{
				if (PC->GetPawn())
				{
					AttributesComponent = PC->GetPawn()->FindComponentByClass<UAttributesComponent>();
				
					AttributesComponent->OnAttributesChangedDelegate.AddDynamic(this, &UEquipmentAttributesWidget::OnGetNotified);
			
					
					EffectsComponent = PC->GetPawn()->FindComponentByClass<UEffectsComponent>();

					if (EffectsComponent)
					{
						EffectsComponent->OnConsumableEffectStartDelegate.AddDynamic(this, &UEquipmentAttributesWidget::DisplayConsumableEffectWidget);
						EffectsComponent->OnEffectEndDelegate.AddDynamic(this, &UEquipmentAttributesWidget::RemoveConsumableEffectWidget);
						EffectsComponent->OnEffectExtendedDelegate.AddDynamic(this, &UEquipmentAttributesWidget::OnEffectExtended);
					}
				}
			
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

void UEquipmentAttributesWidget::DisplayConsumableEffectWidget(FCharacterEffect Effect, FName ItemID)
{
	if (!IsValid(EffectsComponent))
		return;

	const TArray<UActiveEffectInstance*> Effects = EffectsComponent->GetConsumableEffects();
	for (UActiveEffectInstance* E : Effects)
	{
		if (!IsValid(E)) continue;
		GenerateConsumableEffectWidget(E->EffectInstanceID, E->CharacterEffectDefinition.Duration, ItemID);
	}

	HandleBuffTextColor();
}


void UEquipmentAttributesWidget::GenerateConsumableEffectWidget(FGuid EffecInstanceID, float Duration, FName ItemID)
{
	if (ConsumableEffectWidgetComponent)
	{
		UEquipmentConsumableEffectWidget* ConsumableWidget = CreateWidget<UEquipmentConsumableEffectWidget>(GetOwningPlayer(),ConsumableEffectWidgetComponent);
		if (ConsumableWidget)
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green,TEXT("SDDASDAS"));
			const FItemBaseData& BaseItemData = AttributesComponent->GetBaseItemData(ItemID);
			ConsumableWidget->SetData(BaseItemData.ItemIcon, BaseItemData.ItemName,Duration);
			ConsumableEffectWidgetMap.Add(EffecInstanceID, ConsumableWidget);
			
			ConsumableEffects->AddChild(ConsumableWidget);
		}
	}
}

void UEquipmentAttributesWidget::RemoveConsumableEffectWidget(FCharacterEffect Effect, FGuid EffectInstanceID)
{
	for (TPair<FGuid, UEquipmentConsumableEffectWidget*> Pair : ConsumableEffectWidgetMap)
	{
		if (Pair.Key == EffectInstanceID)
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

void UEquipmentAttributesWidget::OnEffectExtended(UActiveEffectInstance* Effect)
{
	for (TPair<FGuid, UEquipmentConsumableEffectWidget*> Pair : ConsumableEffectWidgetMap)
	{
		if (Pair.Key == Effect->EffectInstanceID)
		{
			float dur = EffectsComponent->GetDurationForEffect(Effect->EffectInstanceID);
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