// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Instances/ObjectiveInstance.h"

void UObjectiveInstance::Objective_Initialize(UObjectiveDefinition* Definition)
{
	if (Definition)
	{
		ObjectiveDefinition = Definition;
		
		// UE_LOG(LogTemp, Warning, TEXT("DisplayText: %s"), *Definition->DisplayText.ToString());
		// UE_LOG(LogTemp, Warning, TEXT("Conditions: %d"), Definition->Conditions.Num());
		//
		// for (UConditionDefinition* Condition : Definition->Conditions)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Condition Text: %s"), *Condition->DisplayText.ToString());
		// }
	}
}
