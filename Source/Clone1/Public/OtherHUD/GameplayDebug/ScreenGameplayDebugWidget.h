// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayDebugWidget.h"
#include "Blueprint/UserWidget.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "ScreenGameplayDebugWidget.generated.h"

/**
 * 
 */
class ABaseCharacter;
class AMainPlayerController;
UCLASS()
class CLONE1_API UScreenGameplayDebugWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<ABaseCharacter> CurrentTarget = nullptr;
public:
	
	// Notification
	FTimerHandle NotificationDebugTimer;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NotificationDebug;
	
	
	UPROPERTY(meta=(BindWidget))
	UGameplayDebugWidget* Self_GameplayDebugWidget;
	UPROPERTY(meta=(BindWidget))
	UGameplayDebugWidget* Target_GameplayDebugWidget;
	
	UFUNCTION()
	void HandleTargetBox(ABaseCharacter* TargetChar,FEntityGameplayDebugSnapshot Snapshot);
	UFUNCTION()
	void ToggleTargetBox(bool bState);
	UFUNCTION()
	void DisableTargetBox();
	UFUNCTION()
	void CaptureAbilityFailSnapshot(EAbilityFailureReason& Reason, FGameplayTag AbilityName);
	void SetTimer(FTimerHandle& Handle, float Duration, FTimerDelegate FallbackDelegate);
	UFUNCTION()
	void SetNotificationDebugAndToggle(FText& Text);
	UFUNCTION()
	void SubscribeToTarget(ABaseCharacter* Target);
	UFUNCTION()
	void UnsubscribeFromTarget();
	UFUNCTION()
	void Init(AMainPlayerController* PC);
};
