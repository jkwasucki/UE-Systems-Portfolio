// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "PacmanHUD.generated.h"


class APacmanGame;
/**
 * 
 */
UCLASS()
class CLONE1_API UPacmanHUD : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UFUNCTION()
	void UpdateHealth(int32 CurrentLives);
	UFUNCTION()
	void UpdateScore(int32 Score);
	UFUNCTION()
	void InitializeHUD();
	UFUNCTION()
	void ToggleReadyText();
	UFUNCTION()
	void EnableReadyText();
	UFUNCTION()
	void DisableReadyText();
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ReadyText;
	
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* LivesBox;
	
	UFUNCTION()
	void SetupHUD(APacmanGame* PG);
	
};
