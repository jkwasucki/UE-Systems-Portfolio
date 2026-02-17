// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveDefinition.h"
#include "Engine/DataAsset.h"
#include "QuestDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CLONE1_API UQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bObjectivesSequential;
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TArray<UObjectiveDefinition*> Objectives;
};
