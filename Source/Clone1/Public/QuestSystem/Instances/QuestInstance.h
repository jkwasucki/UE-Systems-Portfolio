// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveInstance.h"
#include "QuestSystem/Definitions/QuestDefinition.h"
#include "Types/QuestTypes.h"
#include "QuestInstance.generated.h"

/**
 * 
 */

UCLASS()
class CLONE1_API UQuestInstance : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UQuestDefinition* QuestDefinition = nullptr;
	UPROPERTY()
	TArray<UObjectiveInstance*> ObjectiveInstances;
	UPROPERTY()
	EQuestState QuestState;
	UPROPERTY()
	FGuid InstanceID;
public:
	UFUNCTION()
	void Initialize(UQuestDefinition* InQuestDefinition);
	UFUNCTION()
	UQuestDefinition* GetQuestDefinition() const;
	UFUNCTION()
	FGuid& GetInstanceID();
	UFUNCTION()
	EQuestState GetQuestState();
	UFUNCTION()
	TArray<UObjectiveInstance*>& GetObjectiveInstances();
};
