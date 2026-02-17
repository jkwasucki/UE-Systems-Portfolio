// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/HUD//WeaponDebugWidget.h"
#include "WeaponSystem/Definitions/WeaponDefinition.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Main/Character/Derived/MainCharacter.h"


void UWeaponDebugWidget::Init(APlayerController* InPC)
{
	if (InPC)
	{
		if (InPC->GetWorld())
		{
			if (AMainCharacter* Character = Cast<AMainCharacter>(InPC->GetPawn()))
			{
				Character->GetDebugComponent()->OnWeaponRuntimeSnapshotDelegate.AddDynamic(this,&UWeaponDebugWidget::UpdateWidget);
			}
		}
	}
}
void UWeaponDebugWidget::UpdateWidget(FWeaponInstanceRuntimeSnapshot Snapshot)
{
	if (GetVisibility() == ESlateVisibility::Hidden)
		SetVisibility(ESlateVisibility::HitTestInvisible);
	
	UpdateRuntimeData(Snapshot);
	
	if (Snapshot.bDefinitionExists)
	{
		if (!bIsStaticBoxDisplayed)
		{
			bIsStaticBoxDisplayed = true;
			StaticData->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		UpdateStaticData(Snapshot);
	}
	else
	{
		if (bIsStaticBoxDisplayed)
		{
			bIsStaticBoxDisplayed = false;
			StaticData->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UWeaponDebugWidget::UpdateRuntimeData(FWeaponInstanceRuntimeSnapshot& Snapshot)
{
	InstanceID->SetText(FText::FromString(Snapshot.InstanceID.ToString()));
	State->SetText(
		StaticEnum<EWeaponState>()
		->GetDisplayNameTextByValue(static_cast<int64>(Snapshot.WeaponState))	
	);
	FireMode->SetText(
		StaticEnum<EWeaponFireMode>()
		->GetDisplayNameTextByValue(static_cast<int64>(Snapshot.WeaponFireMode))	
	);
	HasDefinition->SetText(
			Snapshot.bDefinitionExists ?  FText::FromString("True") : FText::FromString("False")
	);
	HasOwner->SetText(
		Snapshot.bOwnerExists ?  FText::FromString("True") : FText::FromString("False")
	);
}

void UWeaponDebugWidget::UpdateStaticData(FWeaponInstanceRuntimeSnapshot& Snapshot)
{
	UWeaponDefinition* Def = Snapshot.Definition.LoadSynchronous();
	if (Def)
	{
		Name->SetText(
		Def->Name	
	);
		DamageValue->SetText(
			FText::AsNumber(Def->Damage)	
		);
		TBSValue->SetText(
			FText::AsNumber(Def->TimeBetweenShots)
		);
		SpreadValue->SetText(
			FText::AsNumber(Def->Spread)	
		);
		const UEnum* EnumPtr = StaticEnum<EWeaponFireMode>();
		TArray<FString> ModeStrings;

		for (EWeaponFireMode Mode : Snapshot.Definition->FireModes)
		{
			ModeStrings.Add(EnumPtr->GetDisplayNameTextByValue((int64)Mode).ToString());
		}

		FText FireModesText = FText::FromString(
			FString::Printf(TEXT("[%s]"), *FString::Join(ModeStrings, TEXT(", ")))
		);
		FireModes->SetText(FireModesText);
	}
	
}