// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan/PacmanHUD.h"
#include "PacMan/PacmanGame.h"





void UPacmanHUD::SetupHUD(APacmanGame* PG)
{
	if (PG)
	{
		PG->OnGameStoppedDelegate.AddDynamic(this, &UPacmanHUD::EnableReadyText);
		PG->OnGameStartDelegate.AddDynamic(this, &UPacmanHUD::InitializeHUD);
		PG->OnLivesChangedDelegate.AddDynamic(this, &UPacmanHUD::UpdateHealth);
		PG->OnScoreChangedDelegate.AddDynamic(this, &UPacmanHUD::UpdateScore);
	}
}

void UPacmanHUD::UpdateScore(int32 Score)
{
	if (!ScoreText) return;
	if (Score >= 0)
	{
		FNumberFormattingOptions Options;
		Options.UseGrouping = false;

		ScoreText->SetText(FText::AsNumber(Score, &Options));
	}
}

void UPacmanHUD::InitializeHUD()
{
	ToggleReadyText();
	UpdateScore(0);
	UpdateHealth(3);
}

void UPacmanHUD::ToggleReadyText()
{
	if (!ReadyText)return;
	ReadyText->SetVisibility(ReadyText->IsVisible() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UPacmanHUD::EnableReadyText()
{
	if (!ReadyText)return;
	ReadyText->SetVisibility(ESlateVisibility::Visible);
}

void UPacmanHUD::DisableReadyText()
{
	if (!ReadyText)return;
	ReadyText->SetVisibility(ESlateVisibility::Collapsed);
}

void UPacmanHUD::UpdateHealth(int32 CurrentLives)
{
	if (!LivesBox) return;

	TArray<UWidget*> Children = LivesBox->GetAllChildren();
	const int32 MaxLives = Children.Num();

	CurrentLives = FMath::Clamp(CurrentLives, 0, MaxLives);

	for (int32 i = 0; i < MaxLives; i++)
	{
		if (!Children[i]) continue;

		const int32 LivesFromEnd = MaxLives - i;

		if (LivesFromEnd <= CurrentLives)
		{
			Children[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Children[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


