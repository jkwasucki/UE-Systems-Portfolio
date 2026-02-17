// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Instances/QuestInstance.h"

void UQuestInstance::Initialize(UQuestDefinition* InQuestDefinition)
{
	if (InQuestDefinition)
	{
		QuestDefinition = InQuestDefinition;
		QuestState = EQuestState::Active;
		InstanceID = FGuid::NewGuid();
		
		for (UObjectiveDefinition* ObjectiveDefinition : InQuestDefinition->Objectives)
		{
			if (!IsValid(ObjectiveDefinition)) continue;
			
			if (UObjectiveInstance* ObjectiveInstance = NewObject<UObjectiveInstance>(this))
			{
				ObjectiveInstances.Add(ObjectiveInstance);
				ObjectiveInstance->Objective_Initialize(ObjectiveDefinition);
			}
		}
	}
}

UQuestDefinition* UQuestInstance::GetQuestDefinition() const
{
	return QuestDefinition;
}

FGuid& UQuestInstance::GetInstanceID()
{
	return InstanceID;
}

EQuestState UQuestInstance::GetQuestState()
{
	return QuestState;
}

TArray<UObjectiveInstance*>& UQuestInstance::GetObjectiveInstances()
{
	return ObjectiveInstances;
}
