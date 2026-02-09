// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherHUD/GameplayDebug/ScreenGameplayDebugWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Main/Character/Derived/MainCharacter.h"


void UScreenGameplayDebugWidget::Init(AMainPlayerController* PC)
{
	if (PC)
	{
		// TARGET
		PC->OnEntityDebugSnapshotDelegate.AddDynamic(this,&UScreenGameplayDebugWidget::HandleTargetBox);
		PC->OnEntityDebugSnapshot_ExpiredDelegate.AddDynamic(this, &UScreenGameplayDebugWidget::DisableTargetBox);
		
		if (PC->GetPawn())
		{
			if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(PC->GetPawn()))
			{
				BaseCharacter->OnDisplayDebugSnapshots_AllDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAll);
				BaseCharacter->OnStateChangeDelegate.AddDynamic(Self_GameplayDebugWidget,&UGameplayDebugWidget::UpdateState);
				BaseCharacter->OnEffectAppearDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::ShowEffects_OnSelf);
				BaseCharacter->OnEffectExpiredDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::HideEffect_OnSelf);
				BaseCharacter->OnAttributeDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAttributeData);
				BaseCharacter->OnResourcesDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::UpdateResourcesData);
				BaseCharacter->OnAbilityCastDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAbilityData);
				BaseCharacter->OnAbilityCastFailedDebugSnapshotDelegate.AddDynamic(this, &UScreenGameplayDebugWidget::CaptureAbilityFailSnapshot);
			}
			
		}
	}
}
void UScreenGameplayDebugWidget::SetNotificationDebugAndToggle(FText& Text)
{
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindLambda(
		[this]()
		{
			NotificationDebug->SetVisibility(ESlateVisibility::Hidden);
		});
	
	SetTimer(NotificationDebugTimer,1.3f, TimerDelegate);
	
	NotificationDebug->SetText(Text);
	NotificationDebug->SetVisibility(ESlateVisibility::Visible);
}

void UScreenGameplayDebugWidget::SubscribeToTarget(ABaseCharacter* Target)
{
	
	
	UnsubscribeFromTarget();
	
	CurrentTarget = Target;
	CurrentTarget->OnDisplayDebugSnapshots_AllDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAll);
	CurrentTarget->OnStateChangeDelegate.AddDynamic(Target_GameplayDebugWidget,&UGameplayDebugWidget::UpdateState);
	CurrentTarget->OnEffectAppearDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::ShowEffects_OnSelf);
	CurrentTarget->OnAttributeDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAttributeData);
	CurrentTarget->OnResourcesDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::UpdateResourcesData);
	CurrentTarget->OnAbilityCastDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::UpdateAbilityData);
	CurrentTarget->OnEffectExpiredDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UGameplayDebugWidget::HideEffect_OnSelf);
}
void UScreenGameplayDebugWidget::UnsubscribeFromTarget()
{
	if (CurrentTarget == nullptr) return;
	
	CurrentTarget->OnDisplayDebugSnapshots_AllDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::UpdateAll
	);

	CurrentTarget->OnStateChangeDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::UpdateState
	);

	CurrentTarget->OnEffectAppearDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::ShowEffects_OnSelf
	);
	
	CurrentTarget->OnEffectExpiredDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::HideEffect_OnSelf
	);

	CurrentTarget->OnAttributeDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::UpdateAttributeData
	);

	CurrentTarget->OnResourcesDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::UpdateResourcesData
	);

	CurrentTarget->OnAbilityCastDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UGameplayDebugWidget::UpdateAbilityData
	);
	
	CurrentTarget = nullptr;
}

void UScreenGameplayDebugWidget::SetTimer(
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

void UScreenGameplayDebugWidget::HandleTargetBox(ABaseCharacter* TargetChar, FEntityGameplayDebugSnapshot Snapshot)
{
	SubscribeToTarget(TargetChar);
	ToggleTargetBox(true);
	Target_GameplayDebugWidget->UpdateAll(Snapshot);
}

void UScreenGameplayDebugWidget::ToggleTargetBox(bool bState)
{
	Target_GameplayDebugWidget->SetVisibility(bState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UScreenGameplayDebugWidget::DisableTargetBox()
{
	UnsubscribeFromTarget();
	Target_GameplayDebugWidget->ClearEffectsOnSelf();
	Target_GameplayDebugWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UScreenGameplayDebugWidget::CaptureAbilityFailSnapshot(
	EAbilityFailureReason& Reason,
	FGameplayTag AbilityName
)
{
	FText NotificationText;
	const FText ReasonText =
			StaticEnum<EAbilityFailureReason>()
				->GetDisplayNameTextByValue(static_cast<int64>(Reason));
	
	switch (Reason)
	{
	case EAbilityFailureReason::OnCooldown:
		{
			NotificationText = FText::Format(
				NSLOCTEXT("AbilityFail", "OnCooldown",
					"Ability [{0}] is on cooldown"),
					FText::FromName(AbilityName.GetTagName())
			);
			break;
		}

	case EAbilityFailureReason::InvalidTarget:
		{
			NotificationText = FText::Format(
				NSLOCTEXT("AbilityFail", "InvalidTarget",
					"Ability [{0}] has an invalid target"),
					FText::FromName(AbilityName.GetTagName())
			);
			break;
		}

	case EAbilityFailureReason::InsufficientEnergy:
		{
			NotificationText = FText::Format(
				NSLOCTEXT("AbilityFail", "NoEnergy",
					"Not enough energy to use [{0}]"),
					FText::FromName(AbilityName.GetTagName())
			);
			break;
		}

	default:
		return;
	}

	SetNotificationDebugAndToggle(NotificationText);
}
