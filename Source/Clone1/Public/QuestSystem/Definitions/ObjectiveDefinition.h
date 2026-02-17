// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionDefinition.h"
#include "Types/QuestTypes.h"
#include "UObject/NoExportTypes.h"
#include "ObjectiveDefinition.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType, Blueprintable)
class CLONE1_API UObjectiveDefinition : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText DisplayText;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EConditionLogic ConditionsLogicType;
	UPROPERTY(EditAnywhere,Instanced)
	TArray<UConditionDefinition*> Conditions;
};
