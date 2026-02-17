// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherHUD/GameplayDebug/QuestDebugWidget.h"

void UQuestDebugWidget::DisplayQuest(UQuestInstance* NewQuest)
{
	if (UQuestInstanceWidget* QuestInstanceWidget = CreateWidget<UQuestInstanceWidget>(this, QuestInstanceWidgetClass))
	{
		QuestWidgets.Add(QuestInstanceWidget);	
		QuestList->AddChild(QuestInstanceWidget);
		
		QuestInstanceWidget->Init(NewQuest);
	}
}

void UQuestDebugWidget::RemoveQuest(UQuestInstance* Quest)
{
	UQuestInstanceWidget* FoundWidget = nullptr;
	for (UQuestInstanceWidget* Widget : QuestWidgets)
	{
		if (Widget->ObservedQuestInstance == Quest)
		{
			FoundWidget = Widget;
			break;
		}
	}
	
	if (FoundWidget)
	{
		QuestWidgets.Remove(FoundWidget);
		QuestList->RemoveChild(FoundWidget);
	}
}

