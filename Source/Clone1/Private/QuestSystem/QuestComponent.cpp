// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/QuestComponent.h"

UQuestComponent::UQuestComponent()
{
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Start quest after 2 seconds
	GetWorld()->GetTimerManager().SetTimer(
		QuestStartTimerHandle,
		this,
		&UQuestComponent::StartDebugQuest,
		2.0f,
		false
	);
}

void UQuestComponent::AddQuest(UQuestDefinition* InQuestDefinition)
{
	if (UQuestInstance* QuestInstance = NewObject<UQuestInstance>(this))
	{
			QuestInstance->Initialize(InQuestDefinition);
			ActiveQuests.Add(QuestInstance);
			OnQuestStartedDelegate.Broadcast(QuestInstance);
	}
}
void UQuestComponent::StartDebugQuest()
{
	AddQuest(DEBUG_SomeQuest);
}
void UQuestComponent::RemoveQuest(UQuestInstance* InQuestInstance)
{
	for (UQuestInstance* QuestInstance : ActiveQuests)
	{
		if (!IsValid(QuestInstance)) continue;
		
		if (QuestInstance == InQuestInstance)
		{
			ActiveQuests.Remove(QuestInstance);
			OnQuestEndedDelegate.Broadcast(InQuestInstance);
			return;
		}
	}
}

