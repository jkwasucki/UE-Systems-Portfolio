// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherHUD/GameplayDebug/ScreenGameplayDebugWidget.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Main/PlayerState/MainPlayerState.h"


void UScreenGameplayDebugWidget::Init(AMainPlayerController* PC)
{
	if (PC)
	{
		AMainCharacter* MC = PC->GetMainCharacter();
		// TARGET
		MC->GetDebugComponent()->OnEntityDebugSnapshotDelegate.AddDynamic(this,&UScreenGameplayDebugWidget::HandleTargetBox);
		MC->GetInteractionComponent()->OnNoEntityUnderCursorDelegate.AddDynamic(this, &UScreenGameplayDebugWidget::DisableTargetBox);
		
		if (PC->GetPawn())
		{
			if (AMainPlayerState* MainState = Cast<AMainPlayerState>(PC->GetPawn()->GetPlayerState()))
			{
				if (UQuestComponent* QuestSystemComponent = MainState->QuestSystemComponent)
				{
					QuestSystemComponent->OnQuestStartedDelegate.AddDynamic(QuestDebugWidget, &UQuestDebugWidget::DisplayQuest);
				}
			}
			
			if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(PC->GetPawn()))
			{
				BaseCharacter->GetDebugComponent()->OnDisplayDebugSnapshots_AllDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAll);
				BaseCharacter->OnStateChangeDelegate.AddDynamic(Self_GameplayDebugWidget,&UCoreAndAbilityDebugWidget::UpdateState);
				BaseCharacter->GetDebugComponent()->OnEffectAppearDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::ShowEffects_OnSelf);
				BaseCharacter->GetDebugComponent()->OnEffectExpiredDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::HideEffect_OnSelf);
				BaseCharacter->GetDebugComponent()->OnAttributeDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAttributeData);
				BaseCharacter->GetDebugComponent()->OnResourcesDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateResourcesData);
				BaseCharacter->GetDebugComponent()->OnAbilityCastDebugSnapshotDelegate.AddDynamic(Self_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAbilityData);
				BaseCharacter->GetDebugComponent()->OnAbilityCastFailedDebugSnapshotDelegate.AddDynamic(this, &UScreenGameplayDebugWidget::CaptureAbilityFailSnapshot);
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
	CurrentTarget->GetDebugComponent()->OnDisplayDebugSnapshots_AllDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAll);
	CurrentTarget->OnStateChangeDelegate.AddDynamic(Target_GameplayDebugWidget,&UCoreAndAbilityDebugWidget::UpdateState);
	CurrentTarget->GetDebugComponent()->OnEffectAppearDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::ShowEffects_OnSelf);
	CurrentTarget->GetDebugComponent()->OnAttributeDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAttributeData);
	CurrentTarget->GetDebugComponent()->OnResourcesDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateResourcesData);
	CurrentTarget->GetDebugComponent()->OnAbilityCastDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::UpdateAbilityData);
	CurrentTarget->GetDebugComponent()->OnEffectExpiredDebugSnapshotDelegate.AddDynamic(Target_GameplayDebugWidget, &UCoreAndAbilityDebugWidget::HideEffect_OnSelf);
}
void UScreenGameplayDebugWidget::UnsubscribeFromTarget()
{
	if (CurrentTarget == nullptr) return;
	
	CurrentTarget->GetDebugComponent()->OnDisplayDebugSnapshots_AllDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::UpdateAll
	);

	CurrentTarget->OnStateChangeDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::UpdateState
	);

	CurrentTarget->GetDebugComponent()->OnEffectAppearDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::ShowEffects_OnSelf
	);
	
	CurrentTarget->GetDebugComponent()->OnEffectExpiredDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::HideEffect_OnSelf
	);

	CurrentTarget->GetDebugComponent()->OnAttributeDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::UpdateAttributeData
	);

	CurrentTarget->GetDebugComponent()->OnResourcesDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::UpdateResourcesData
	);

	CurrentTarget->GetDebugComponent()->OnAbilityCastDebugSnapshotDelegate.RemoveDynamic(
		Target_GameplayDebugWidget,
		&UCoreAndAbilityDebugWidget::UpdateAbilityData
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
