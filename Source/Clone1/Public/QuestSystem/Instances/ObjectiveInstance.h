// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystem/Definitions/ObjectiveDefinition.h"
#include "UObject/NoExportTypes.h"
#include "ObjectiveInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UObjectiveInstance : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UObjectiveDefinition* ObjectiveDefinition = nullptr;
	
	
public:
	UFUNCTION()
	void Objective_Initialize(UObjectiveDefinition* Definition);
};
