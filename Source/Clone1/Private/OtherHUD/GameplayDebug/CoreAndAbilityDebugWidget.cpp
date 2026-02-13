// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherHUD/GameplayDebug/CoreAndAbilityDebugWidget.h"

#include "AbilitySystem/Instances/ActiveEffectInstance.h"


void UCoreAndAbilityDebugWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (GetWorld() &&
	GetWorld()->GetTimerManager().IsTimerActive(AbilityBoxVisibilityTimer))
	{
		float RemainingCooldown = 0.f;
		if (GetWorld())
		{
			RemainingCooldown = GetWorld()->GetTimerManager()
				.GetTimerRemaining(AbilityBoxVisibilityTimer);
		}

		CooldownRemainingValue->SetText(
			FText::FromString(
				FString::Printf(TEXT("%.2f s"), FMath::Max(0.f, RemainingCooldown))
			)
		);
	}
}


void UCoreAndAbilityDebugWidget::SetTimer(
	FTimerHandle& Handle,
	float Duration,
	FTimerDelegate FallbackDelegate
)
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);

	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FallbackDelegate,
		Duration,
		false
	);
}

void UCoreAndAbilityDebugWidget::UpdateResourcesData(FResourceDebugSnapshot& Snapshot)
{
	HealthValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Health)
		)
	);
	
	EnergyValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Energy)
		)
	);
	
}

void UCoreAndAbilityDebugWidget::UpdateAttributeData(FAttributeDebugSnapshot& Snapshot)
{
	SpeedValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Speed)
		)
	);
	ArmorValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Armor)
		)
	);
	AttackValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Attack)
		)
	);
	HealthAttributeValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.Health)
		)
	);
}

void UCoreAndAbilityDebugWidget::UpdateAll(FEntityGameplayDebugSnapshot& Snapshot)
{
	UpdateAttributeData(Snapshot.AttributeSnapshot);
	UpdateResourcesData(Snapshot.ResourceDebugSnapshot);
	if (Snapshot.AbilityDebugSnapshot.Effects.Num() > 0)
	{
		ShowEffects_OnSelf(Snapshot.AbilityDebugSnapshot);
	}
	
	EntityType->SetText(
	StaticEnum<EEntityType>()
		->GetDisplayNameTextByValue(
			static_cast<int64>(Snapshot.EntityType)
		)
	);
	EntityState->SetText(
	StaticEnum<EEntityState>()
		->GetDisplayNameTextByValue(
			static_cast<int64>(Snapshot.EntityState)
		)
	);
}

void UCoreAndAbilityDebugWidget::UpdateState(EEntityState State)
{
	EntityState->SetText(
	StaticEnum<EEntityState>()
		->GetDisplayNameTextByValue(
			static_cast<int64>(State)
		)
	);
}


void UCoreAndAbilityDebugWidget::UpdateAbilityData(FAbilityDebugSnapshot& Snapshot)
{

	// SET TIMER
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda(
		[this]()
		{
			ToggleAbilityBox(false);
		});
	SetTimer(AbilityBoxVisibilityTimer, Snapshot.Cooldown,TimerDelegate);
	//=======================//=======================//
	//=======================//=======================//
	
	
	// SET PROPERTIES
	CostValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f"), Snapshot.EnergyCost)
		)
	);
	
	AbilityNameValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%s"), *Snapshot.AbilityTag.GetTagName().ToString())
		)
	);
	
	
	CooldownDurationValue->SetText(
		FText::FromString(
			FString::Printf(TEXT("%.2f s"), Snapshot.Cooldown)
		)
	);
	AnimationLockMValue->SetText(
		Snapshot.bLocksMovement
			? FText::FromString(TEXT("Yes"))
			: FText::FromString(TEXT("No"))
	);
	// AbilityNameValue->SetText(
	// 	StaticEnum<EAbilityName>()
	// 		->GetDisplayNameTextByValue(
	// 			static_cast<int64>(Snapshot.AbilityName)
	// 		)
	// );
	AbilityModeValue->SetText(
	StaticEnum<EAbilityCastMode>()
		->GetDisplayNameTextByValue(
			static_cast<int64>(Snapshot.AbilityCastMode)
		)
	);
	AnimationModeValue->SetText(
		StaticEnum<EAnimationBodyPart>()->GetDisplayNameTextByValue(
			static_cast<int64>(Snapshot.AnimationMode)
		)
	);
	TargetingValue->SetText(FText::FromString(Snapshot.TargetingStrategy));
	//=======================//=======================//
	//=======================//=======================//
	//
	// SPAWN ABILITY EFFECTS DESCRIPTION
	AbilityEffectsList->ClearChildren();
	
	ShowEffects_OnAbility(Snapshot);
	//=======================//=======================//
	//=======================//=======================//
	
	ToggleAbilityBox(true);
}


UTextBlock* UCoreAndAbilityDebugWidget::SpawnEffectDescriptor(FText& Text,UVerticalBox* Box)
{
	UTextBlock* TextBlock = NewObject<UTextBlock>(this);
	if (!TextBlock)
	{
		return nullptr;
	}
	
	TextBlock->SetText(Text);

	Box->AddChild(TextBlock);
	return TextBlock;
}




void UCoreAndAbilityDebugWidget::ShowEffects_OnSelf(FAbilityDebugSnapshot& Ability)
{
	for (FCharacterEffect& Effect : Ability.Effects)
	{
		if (Effect.EffectMode  == EEffectMode::InstantPersistent) return;
		if (Effect.bCanStack)
		{
	
		}
	
		FText T = FormatEffectDescriptor(Effect);
		UTextBlock* TB = SpawnEffectDescriptor(T,EffectsOnSelfList);
		FSlateFontInfo FontInfo = TB->GetFont();
		FontInfo.Size = 10;
		TB->SetFont(FontInfo);
		DisplayedEffectsOnPlayer.Add(Effect.TypeID, TB);
		
	}
}

void UCoreAndAbilityDebugWidget::ShowEffects_OnAbility(FAbilityDebugSnapshot& Ability)
{
	for (FCharacterEffect& Effect : Ability.Effects)
	{
		FText T = FormatEffectDescriptor(Effect);
		UTextBlock* TB = SpawnEffectDescriptor(T,AbilityEffectsList);
		FSlateFontInfo FontInfo = TB->GetFont();
		FontInfo.Size = 10;
		TB->SetFont(FontInfo);
	}
}

void UCoreAndAbilityDebugWidget::HandleDisplayEffect(FAbilityDebugSnapshot& Ability)
{
	for (FCharacterEffect& Effect : Ability.Effects)
	{
		if (!DisplayedEffectsOnPlayer.Contains(Effect.TypeID))
		{
			ShowEffects_OnSelf(Ability);
		}
		else
		{
			HideEffect_OnSelf(Ability);
		}
	}
}

void UCoreAndAbilityDebugWidget::HideEffect_OnSelf(FAbilityDebugSnapshot& Ability)
{
	for (FCharacterEffect& Effect : Ability.Effects)
	{
		if (UTextBlock** Found = DisplayedEffectsOnPlayer.Find(Effect.TypeID))
		{
			(*Found)->RemoveFromParent();
			DisplayedEffectsOnPlayer.Remove(Effect.TypeID);
		}
	}
}

void UCoreAndAbilityDebugWidget::ClearEffectsOnSelf()
{
	DisplayedEffectsOnPlayer.Empty();
	EffectsOnSelfList->ClearChildren();
}


FText UCoreAndAbilityDebugWidget::FormatEffectDescriptor(FCharacterEffect& Effect)
{
	FText ModeText = StaticEnum<EEffectMode>()
		->GetDisplayNameTextByValue((int64)Effect.EffectMode);

	FText TypeText = StaticEnum<EEffectType>()
		->GetDisplayNameTextByValue((int64)Effect.EffectType);

	FText TargetingResource = StaticEnum<ECharacterResource>()
		->GetDisplayNameTextByValue((int64)Effect.TargetedResource);

	FText TargetingAttribute = StaticEnum<EAttribute>()
		->GetDisplayNameTextByValue((int64)Effect.TargetedAttribute);

	FFormatNamedArguments Args;
	Args.Add(TEXT("Value"), FText::AsNumber(Effect.RawValue));
	Args.Add(TEXT("Mode"), ModeText);
	Args.Add(TEXT("Type"), TypeText);
	Args.Add(TEXT("ID"), FText::FromName(Effect.TypeID));
	Args.Add(TEXT("Resource"), TargetingResource);
	Args.Add(TEXT("Attribute"), TargetingAttribute);

	FText FormattedDesc = FText::Format(
		FText::FromString(
			TEXT("[{ID}] | Value: [{Value}] | TR: [{Resource}] | TA: [{Attribute}] | Type: [{Type}] | Mode: [{Mode}]")
		),
		Args
	);
	
	return FormattedDesc;
}

void UCoreAndAbilityDebugWidget::ToggleAbilityBox(bool bState)
{
	AbilityBox->SetVisibility(bState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}




