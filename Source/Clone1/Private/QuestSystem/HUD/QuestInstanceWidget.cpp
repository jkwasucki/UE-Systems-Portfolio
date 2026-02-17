// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/HUD/QuestInstanceWidget.h"

#include "QuestSystem/Instances/ObjectiveInstance.h"


void UQuestInstanceWidget::Init(UQuestInstance* QuestInstance)
{
	if (QuestInstance)
	{
		ObservedQuestInstance = QuestInstance;
		//Sub to events
		SetData();
		DisplayObjectives();
		// Fill ui from definition
	}
}

void UQuestInstanceWidget::DisplayObjectives()
{
	if (ObservedQuestInstance == nullptr) return;
	
	TArray<UObjectiveInstance*>& Objectives = ObservedQuestInstance->GetObjectiveInstances();
	
	for (UObjectiveInstance* ObjectiveInstance : Objectives)
	{
		if (UObjectiveInstanceWidget* ObjectiveInstanceWidget = CreateWidget<UObjectiveInstanceWidget>(this,ObjectiveInstanceWidgetClass))
		{
			ObjectiveInstanceWidgets.Add(ObjectiveInstanceWidget);
			ObjectivesList->AddChild(ObjectiveInstanceWidget);
			// ObjectiveInstanceWidget->Initialize(ObjectiveInstance->Obje);
		}
	}
}

void UQuestInstanceWidget::SetData()
{
	QuestName->SetText(ObservedQuestInstance->GetQuestDefinition()->DisplayName);
	ID->SetText(FText::FromString(ObservedQuestInstance->GetInstanceID().ToString(EGuidFormats::Digits).Left(8)));
	State->SetText(StaticEnum<EQuestState>()->GetDisplayNameTextByValue(
		(int64)ObservedQuestInstance->GetQuestState()
	));
	
}

